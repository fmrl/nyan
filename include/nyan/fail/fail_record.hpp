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

#include <nyan/backtrace.hpp>
#include <nyan/ptr.hpp>
#include <nyan/source_coordinate.hpp>

#include <nyan/config.h>
#if NYAN_CAN_HAS_YAML
#     include <yaml-cpp/yaml.h>
#endif // NYAN_CAN_HAS_YAMLCPP

#include <boost/regex.hpp>

#include <map>
#include <sstream>
#include <string>
#include <typeinfo>

namespace nyan
{

class fail_record :
   public reference_counted_object
{
private:

   typedef std::map< std::string, std::string > fields_type;
   typedef boost::regex regex_type;
   typedef boost::smatch match_type;

   static const regex_type our_subst_re;

   fields_type my_fields;

public:

   static const std::string our_type_field;
   static const std::string our_summary_field;
   static const std::string our_where_field;
   static const std::string our_backtrace_field;

   fail_record(const source_coordinate &where_arg,
         const std::string &summary_arg);

   virtual ~fail_record() throw();

   template < class Value >
   void sto(const std::string &name_arg, const Value &value_arg)
   {
      std::ostringstream s;
      s << value_arg;
      sto_string(name_arg, s.str());
   }

   void sto(const std::string &name_arg, const std::string &value_arg)
   {
      sto_string(name_arg, value_arg);
   }

   void sto(const std::string &name_arg, const char *value_arg)
   {
      sto_string(name_arg, value_arg);
   }

   void sto(const std::string &name_arg,
         const source_coordinate &where_arg)
   {
      sto_source_coordinate(name_arg, where_arg);
   }

   void sto_string(const std::string &name_arg,
         const std::string &value_arg);
   void sto_string(const std::string &name_arg,
         const char *value_arg);
   void sto_source_coordinate(const std::string &name_arg,
         const source_coordinate &where_arg);
   template < class Object >
   void sto_typename(const std::string &name_arg, const Object *ptr_arg)
   {
      return sto_string(name_arg, typeid(ptr_arg).name());
   }

   const std::string & rcl(const std::string &name_arg) const;
   void print(std::ostream &out_arg,
         const std::string &name_arg) const;

   void summary(const std::string &text_arg);
   const std::string & summary() const;
   void print_summary(std::ostream &out_arg) const;

   void where(const source_coordinate &where_arg);
   const std::string & where() const;
   void print_where(std::ostream &out_arg) const;

   template < class Object >
   void type(const Object *ptr_arg)
   {
      return sto_typename(our_type_field, ptr_arg);
   }
   const std::string & type() const;
   void print_type(std::ostream &out_arg) const;

   void backtrace(const std::string &backtrace_arg);
   void backtrace(const ::nyan::backtrace &backtrace_arg);
   const std::string & backtrace() const;
   void print_backtrace(std::ostream &out_arg) const;

#if NYAN_CAN_HAS_YAML
   void emit_yaml(YAML::Emitter &out_arg) const;
   void emit(YAML::Emitter &out_arg) const;
#endif

private:

   void initialize(const source_coordinate &where_arg,
         const std::string &summary_arg);

};

}

#endif // NYAN_FAIL_RECORD_HPP_IS_INCLUDED
