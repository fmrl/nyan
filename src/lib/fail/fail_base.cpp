/* ex: set softtabstop=3 shiftwidth=3 expandtab: */

/* This file is part of the *nyan* project at <http://fmrl.org>.
 * Copyright (c) 2011, Michael Lowell Roberts.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 *  - Neither the name of the copyright holder nor the names of
 *  contributors may be used to endorse or promote products derived
 *  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <nyan/fail/fail_base.hpp>

#include <nyan/fail.hpp>
#include <nyan/panic.hpp>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

namespace nyan
{

const std::string fail::our_type_field_name("type");
const std::string fail::our_summary_field_name("summary");
const std::string fail::our_where_field_name("where");
const std::string fail::our_backtrace_field_name("backtrace");

// match index 0 will hold the entire string matched.
// match index 1 will refer to "\$", if detected.
// match index 2 will refer to "${varname}", if detected.
// if match index 2 is not empty, position 3 will hold "varname".
const fail::field::regex_type
   fail::field::our_subst_re("(\\\\\\$)|(\\$\\{([^$]+)\\})");

const std::string fail::field::our_nil_string("nil");

fail::field::field() :
   my_pair(our_nil_string, our_nil_string)
{}

fail::field::~field() throw()
{}

const std::string & fail::field::name() const
{
   return my_pair.first;
}

const std::string & fail::field::value() const
{
   return my_pair.second;
}

void fail::field::print(std::ostream &out_arg,
      const fail_record &record_arg) const
{
   boost::sregex_iterator end;
   boost::sregex_iterator i(value().begin(),
         value().end(), our_subst_re);
   // *j* will track the last character matched by the regex iterator.
   // this is important because we need to know how much to append to the
   // end of the output to make our task complete.
   std::string::size_type j = 0;
   for (; i != end; ++i)
   {
      out_arg << i->prefix();
      j += i->prefix().length() + (*i)[0].length();
      if ((*i)[1].matched)
         out_arg << "$";
      else
      {
         const std::string * const s = &record_arg.at((*i)[3]);
         out_arg << *s;
      }
   }

   out_arg << value().substr(j, value().npos);
}

const fail::field::pair_type
      fail::field::make_pair(const std::string &name_arg,
      const std::string &value_arg)
{
   NYAN_FAIL_IFEMPTY(name_arg);
   NYAN_FAIL_IFEMPTY(value_arg);

   return std::make_pair(name_arg, value_arg);
}

const fail::field::pair_type
      fail::field::make_pair(const std::string &name_arg,
      const char *value_arg)
{
   NYAN_FAIL_IFEMPTY(value_arg);

   return make_pair(name_arg, std::string(value_arg));
}

const fail::field::pair_type
      fail::field::make_pair(const std::string &name_arg,
      const source_coordinate &where_arg)
{
   // [todo] i would like to eliminate string building such as this by
   // implementing a recursive string macro evaluator, which shouldn't be
   // too difficult. i would just need to add code that detects when a
   // specific macro has been expanded recursively. this seems fairly low
   // priority, however, given that i'm going to be generating backtraces
   // for each exception constructed.
   std::ostringstream txt;
   where_arg.print_friendly(txt);
   return make_pair(name_arg, txt.str());
}

const fail::field::pair_type
      fail::field::make_pair(const std::string &name_arg,
      const std::type_info &info_arg)
{
   // warning: iirc, on windows, the demangling is automatic and
   // consequentially *not* thread-safe.
   demangle f;
   std::string s;
   NYAN_PANIC_IF(!f(s, info_arg.name()));
   return make_pair(name_arg, s);
}

void fail::field::fail_if_empty(const char *str_arg)
{
   NYAN_FAIL_IFEMPTY(str_arg);
}

#if NYAN_CAN_HAS_YAML

void fail::field::emit_yaml(YAML::Emitter &out_arg) const
{
   out_arg << YAML::Key << my_pair.first;
   out_arg << YAML::Value;

   // i'll convert strings to sequences of strings, using newlines
   // as a separator. if there's no newlines, then i'll emit a simple
   // scalar.
   if (std::string::npos == my_pair.second.find('\n'))
      out_arg << my_pair.second;
   else
   {
      boost::char_separator< char > delim("\n");
      boost::tokenizer< boost::char_separator< char > >
         tokens(my_pair.second, delim);
      out_arg << YAML::BeginSeq;
      BOOST_FOREACH(const std::string &s, tokens)
      {
         out_arg << s;
      }
      out_arg << YAML::EndSeq;
   }
}

void fail::field::emit(YAML::Emitter &out_arg) const
{
   emit_yaml(out_arg);
}

#endif //NYAN_CAN_HAS_YAML

fail::fail(const source_coordinate &where_arg,
      const std::string &summary_arg) :
      my_record(new fail_record(where_arg, summary_arg))
{
   initialize();
}

fail::~fail() throw()
{}

fail::const_record_ptr_type fail::const_record() const
{
   return my_record;
}

fail::record_ptr_type fail::record() const
{
   return my_record;
}


const char * fail::what() const throw()
{
   try
   {
      std::ostringstream msg;
      print_summary(msg);
      my_what_cache = msg.str();
      return my_what_cache.c_str();
   }
   catch (const std::exception &e)
   {
      std::ostringstream msg;
      msg << "i caught a secondary exception in fail::what(): " << e.what();
      my_what_cache = msg.str();
      return my_what_cache.c_str();
   }
}

const fail::field fail::type(const std::type_info &type_arg)
{
   return field(our_type_field_name, type_arg);
}

const fail::field fail::summary(const std::string &text_arg)
{
   return field(our_summary_field_name, text_arg);
}

const fail::field fail::where(const source_coordinate &where_arg)
{
   return field(our_where_field_name, where_arg);
}

const fail::field fail::backtrace(const ::nyan::backtrace &bt_arg)
{
   return field(our_backtrace_field_name, bt_arg);
}

void fail::initialize()
{
   insert(type(typeid(*this)));
   insert(backtrace(::nyan::backtrace()));
}

void fail::insert(const field &field_arg) const
{
   return my_record->insert(field_arg);
}

const std::string & fail::at(const std::string &name_arg) const
{
   return my_record->at(name_arg);
}

void fail::print(std::ostream &out_arg,
      const std::string &name_arg) const
{
   return my_record->print(out_arg, name_arg);
}

void fail::print_summary(std::ostream &out_arg) const
{
   return my_record->print_summary(out_arg);
}

#if NYAN_CAN_HAS_YAML

void fail::emit_yaml(YAML::Emitter &out_arg) const
{
   std::ostringstream msg;
   print_summary(msg);
   out_arg << YAML::Comment(msg.str());
   my_record->emit_yaml(out_arg);
}

void fail::emit(YAML::Emitter &out_arg) const
{
   emit_yaml(out_arg);
}

YAML::Emitter & operator<<(YAML::Emitter &out_arg, const fail &fail_arg)
{
   fail_arg.emit_yaml(out_arg);
   return out_arg;
}

#endif //CAN_HAS_YAML

}
