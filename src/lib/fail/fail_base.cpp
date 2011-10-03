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
#include <nyan/backtrace.hpp>

namespace nyan
{

fail::fail(const source_coordinate &where_arg,
      const std::string &summary_arg) :
      my_record(new fail_record(where_arg, summary_arg))
{
   initialize();
}

fail::~fail() throw()
{}

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

void fail::initialize()
{
   my_record->type(this);
   my_record->backtrace(::nyan::backtrace());
}

fail & fail::summary(const std::string &text_arg)
{
   my_record->summary(text_arg);
   return *this;
}

void fail::print_summary(std::ostream &out_arg) const
{
   return my_record->print_summary(out_arg);
}

fail & fail::where(const source_coordinate &where_arg)
{
   my_record->where(where_arg);
   return *this;
}

void fail::print_where(std::ostream &out_arg) const
{
   return my_record->print_where(out_arg);
}

void fail::print_type(std::ostream &out_arg) const
{
   return my_record->print_type(out_arg);
}

fail & fail::backtrace(const std::string &text_arg)
{
   my_record->summary(text_arg);
   return *this;
}

void fail::print_backtrace(std::ostream &out_arg) const
{
   return my_record->print_backtrace(out_arg);
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
