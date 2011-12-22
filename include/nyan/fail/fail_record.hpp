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

#ifndef NYAN_FAIL_RECORD_HPP_IS_INCLUDED
#define NYAN_FAIL_RECORD_HPP_IS_INCLUDED

#include <nyan/fail.hpp>
#include <nyan/text_coordinate.hpp>

#include <nyan/config.h>
#if NYAN_CAN_HAS_YAML
#     include <yaml-cpp/yaml.h>
#endif // NYAN_CAN_HAS_YAMLCPP

#include <map>
#include <string>

namespace nyan
{

class fail_record :
   public reference_counted_object
{
private:

   typedef std::map< std::string, fail::field > fields_type;
   fields_type my_fields;

public:

   fail_record(const text_coordinate &where_arg,
         const std::string &summary_arg);

   virtual ~fail_record() throw();

   void insert(const fail::field &field_arg);
   const std::string & at(const std::string &name_arg) const;
   void print(std::ostream &out_arg,
         const std::string &name_arg) const;
   void print_summary(std::ostream &out_arg) const;

#if NYAN_CAN_HAS_YAML
   void emit_yaml(YAML::Emitter &out_arg) const;
   void emit(YAML::Emitter &out_arg) const;
#endif

private:

   void initialize(const text_coordinate &where_arg,
         const std::string &summary_arg);

};//class fail_record

}

#endif // NYAN_FAIL_RECORD_HPP_IS_INCLUDED
