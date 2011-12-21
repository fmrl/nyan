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

#include <boost/foreach.hpp>

namespace nyan
{

fail_record::fail_record(const source_coordinate &where_arg,
      const std::string &summary_arg)
{
   initialize(where_arg, summary_arg.c_str());
}

fail_record::~fail_record() throw()
{}

void fail_record::insert(const fail::field &field_arg)
{
   my_fields[field_arg.name()] = field_arg;
}

const std::string & fail_record::at(const std::string &name_arg) const
{
   fields_type::const_iterator i = my_fields.find(name_arg);
   if (my_fields.end() == i)
      throw not_found_fail(NYAN_HERE(), name_arg);
   else
      return i->second.value();
}

void fail_record::print(std::ostream &out_arg,
      const std::string &name_arg) const
{
   fields_type::const_iterator i = my_fields.find(name_arg);
   if (my_fields.end() == i)
      throw not_found_fail(NYAN_HERE(), name_arg);
   else
   {
      i->second.print(out_arg, *this);
   }
}

void fail_record::initialize(const source_coordinate &where_arg,
      const std::string &summary_arg)
{
   insert(fail::where(where_arg));
   insert(fail::summary(summary_arg));
}

void fail_record::print_summary(std::ostream &out_arg) const
{
   return print(out_arg, fail::our_summary_field_name);
}

#if NYAN_CAN_HAS_YAML

void fail_record::emit_yaml(YAML::Emitter &out_arg) const
{
   out_arg << YAML::BeginMap;
   BOOST_FOREACH(const fields_type::value_type &i, my_fields)
   {
      i.second.emit_yaml(out_arg);
   }
   out_arg << YAML::EndMap;
}

void fail_record::emit(YAML::Emitter &out_arg) const
{
   emit_yaml(out_arg);
}

#endif //NYAN_CAN_HAS_YAML

}
