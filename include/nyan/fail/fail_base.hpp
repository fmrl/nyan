// $legal:1626:
// 
// Copyright (c) 2011, Michael Lowell Roberts.  
// All rights reserved. 
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are 
// met: 
// 
//   - Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer. 
// 
//   - Redistributions in binary form must reproduce the above copyright 
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the distribution.
//  
//   - Neither the name of the copyright holder nor the names of 
//   contributors may be used to endorse or promote products derived 
//   from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER 
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
// 
// ,$

#ifndef NYAN_FAIL_BASE_HPP_IS_INCLUDED
#define NYAN_FAIL_BASE_HPP_IS_INCLUDED

#include <nyan/debug.hpp>
#include <nyan/ptr.hpp>
#include <nyan/text_coordinate.hpp>
#include <nyan/yaml.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/regex.hpp>
#include <boost/type_traits.hpp>

#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>

namespace nyan
{
// a forward declaration of fail_record is necessary for it to be
// passed as an argument to field::print().
class fail_record;

class fail :
   public std::exception
{
public:

   class field
   {
   private:

      typedef boost::regex regex_type;
      typedef boost::smatch match_type;
      static const regex_type our_subst_re;
      static const std::string our_nil_string;

      typedef std::pair< std::string, std::string > pair_type;
      pair_type my_pair;

   public:

      field();

      template < class Name, class Value >
      field(const Name &name_arg, const Value &value_arg) :
         my_pair(make_pair(name_arg, value_arg))
      {}

      virtual ~field() throw();

      const std::string & name() const;
      const std::string & value() const;
      void print(std::ostream &out_arg,
            const fail_record &record_arg) const;

#if NYAN_CAN_HAS_YAML
      void emit_yaml(YAML::Emitter &out_arg) const;
      void emit(YAML::Emitter &out_arg) const;
#endif

   private:

      template < class Value >
      static const pair_type make_pair(const char *name_arg,
            const Value &value_arg)
      {
         fail_if_empty(name_arg);
         return make_pair(std::string(name_arg), value_arg);
      }

      template < class Value >
      static const pair_type make_pair(const std::string &name_arg,
            const Value &value_arg)
      {
         std::ostringstream s;
         s << value_arg;
         return make_pair(name_arg, s.str());
      }

      static const pair_type make_pair(const std::string &name_arg,
            const std::string &value_arg);
      static const pair_type make_pair(const std::string &name_arg,
            const char *value_arg);
      static const pair_type make_pair(const std::string &name_arg,
            const text_coordinate &where_arg);
      static const pair_type make_pair(const std::string &name_arg,
            const std::type_info &type_arg);

      static void fail_if_empty(const char *str_arg);

   };//class field

   typedef const_trusted_smart_ptr< fail_record > const_record_ptr_type;
   typedef trusted_smart_ptr< fail_record > record_ptr_type;

   static const std::string our_type_field_name;
   static const std::string our_summary_field_name;
   static const std::string our_where_field_name;
   static const std::string our_backtrace_field_name;

private:

   record_ptr_type my_record;
   mutable std::string my_what_cache;

protected:

   fail(const text_coordinate &where_arg,
         const std::string &summary_arg);

public:

   virtual ~fail() throw();

   virtual const char * what() const throw();

   const_record_ptr_type const_record() const;
   record_ptr_type record() const;

   // insert() needs to be **const** so that
   // operator<< <Other, fail:field>() can successfully match against a
   // newly constructed exception in a **throw** statement.
   void insert(const field &field_arg) const;
   const std::string & at(const std::string &name_arg) const;
   void print(std::ostream &out_arg,
         const std::string &name_arg) const;
   void print_summary(std::ostream &out_arg) const;

   static const field type(const std::type_info &type_arg);
   static const field summary(const std::string &text_arg);
   static const field where(const text_coordinate &where_arg);
   static const field backtrace(const ::nyan::backtrace &bt_arg);

   #if NYAN_CAN_HAS_YAML
   void emit_yaml(YAML::Emitter &out_arg) const;
   void emit(YAML::Emitter &out_arg) const;
#endif

private:

   void initialize();

};

#if NYAN_CAN_HAS_YAML
YAML::Emitter & operator<<(YAML::Emitter &out_arg, const fail &fail_arg);
#endif

template < class Object >
const Object & operator<<(const Object &fail_arg,
      const nyan::fail::field &field_arg)
{
   BOOST_MPL_ASSERT((boost::is_base_of< fail, Object >));

   fail_arg.insert(field_arg);
   return fail_arg;
}

}

#endif // NYAN_FAIL_BASE_HPP_IS_INCLUDED

// $vim:23: vim:set sts=3 sw=3 et:,$
