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

#include <nyan/fail/fail_record.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

// match index 0 will hold the entire string matched.
// match index 1 will refer to "\$", if detected.
// match index 2 will refer to "${varname}", if detected.
// if match index 2 is not empty, position 3 will hold "varname".
const fail_record::regex_type
   fail_record::our_subst_re("(\\\\\\$)|(\\$\\{([^$]+)\\})");

const std::string fail_record::our_type_field("type");
const std::string fail_record::our_summary_field("summary");
const std::string fail_record::our_where_field("where");
const std::string fail_record::our_backtrace_field("backtrace");

fail_record::fail_record(const source_coordinate &where_arg,
      const std::string &summary_arg)
{
   initialize(where_arg, summary_arg.c_str());
}

fail_record::fail_record(const source_coordinate &where_arg,
      const char *summary_arg)
{
   initialize(where_arg, summary_arg);
}

fail_record::~fail_record() throw()
{}

void fail_record::sto_string(const std::string &name_arg,
      const std::string &value_arg)
{
   NYAN_FAIL_IFEMPTY(name_arg);
   NYAN_FAIL_IFEMPTY(value_arg);

   my_fields[name_arg] = value_arg;
}

void fail_record::sto_string(const std::string &name_arg,
      const char *value_arg)
{
   NYAN_FAIL_IFEMPTY(name_arg);
   NYAN_FAIL_IFEMPTY(value_arg);

   my_fields[name_arg] = value_arg;
}

const std::string & fail_record::rcl(const std::string &name_arg) const
{
   fields_type::const_iterator i = my_fields.find(name_arg);
   if (my_fields.end() == i)
      throw not_found_fail(NYAN_HERE(), name_arg);
   else
      return i->second;
}

void fail_record::print(std::ostream &out_arg,
      const std::string &name_arg) const
{
   const std::string * const expr = &rcl(name_arg);

   boost::sregex_iterator end;
   boost::sregex_iterator i(expr->begin(), expr->end(), our_subst_re);
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
         const std::string * const s = &rcl((*i)[3]);
         out_arg << *s;
      }
   }

   out_arg << expr->substr(j, expr->npos);
}

void fail_record::sto_source_coordinate(const std::string &name_arg,
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
   return sto(name_arg, txt.str());
}

void fail_record::initialize(const source_coordinate &where_arg,
      const char *summary_arg)
{
   NYAN_FAIL_IFNULL(summary_arg);

   where(where_arg);
   summary(summary_arg);
}

void fail_record::summary(const std::string &text_arg)
{
   sto(our_summary_field, text_arg);
}

const std::string & fail_record::summary() const
{
   return rcl(our_summary_field);
}

void fail_record::print_summary(std::ostream &out_arg) const
{
   print(out_arg, our_summary_field);
}

void fail_record::where(const source_coordinate &where_arg)
{
   sto(our_where_field, where_arg);
}

const std::string & fail_record::where() const
{
   return rcl(our_where_field);
}

void fail_record::print_where(std::ostream &out_arg) const
{
   print(out_arg, our_where_field);
}

const std::string & fail_record::type() const
{
   return rcl(our_type_field);
}

void fail_record::print_type(std::ostream &out_arg) const
{
   print(out_arg, our_type_field);
}

void fail_record::backtrace(const std::string &backtrace_arg)
{
   sto(our_backtrace_field, backtrace_arg);
}

void fail_record::backtrace(const ::nyan::backtrace &backtrace_arg)
{
   sto(our_backtrace_field, backtrace_arg);
}

const std::string & fail_record::backtrace() const
{
   return rcl(our_backtrace_field);
}

void fail_record::print_backtrace(std::ostream &out_arg) const
{
   out_arg << backtrace();
}

}
