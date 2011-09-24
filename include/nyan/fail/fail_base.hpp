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

#ifndef NYAN_FAIL_BASE_HPP_IS_INCLUDED
#define NYAN_FAIL_BASE_HPP_IS_INCLUDED

#include <nyan/fail/fail_record.hpp>
#include <nyan/ptr.hpp>
#include <nyan/source_coordinate.hpp>

#include <exception>
#include <map>
#include <string>

namespace nyan
{

class fail :
   public std::exception
{
public:

   typedef const_surely_ptr< fail_record > const_record_ptr_type;
   typedef surely_ptr< fail_record > record_ptr_type;

private:

   record_ptr_type my_record;
   mutable std::string my_what_cache;

protected:

   fail(const source_coordinate &where_arg,
         const std::string &summary_arg);
   fail(const source_coordinate &where_arg,
         const char *summary_arg);

public:

   virtual ~fail() throw();

   virtual const char * what() const throw();

   const_record_ptr_type record() const
   {
      return my_record;
   }

   record_ptr_type record()
   {
      return my_record;
   }

   template < class Value >
   fail & sto(const std::string &name_arg, const Value &value_arg)
   {
      my_record->sto(name_arg, value_arg);
      return *this;
   }

   void print(std::ostream &out_arg,
         const std::string &name_arg) const;

   fail & summary(const std::string &text_arg);
   void print_summary(std::ostream &out_arg) const;
   fail & where(const source_coordinate &where_arg);
   void print_where(std::ostream &out_arg) const;
   void print_type(std::ostream &out_arg) const;

private:

   void initialize();

};

}

#endif // NYAN_FAIL_BASE_HPP_IS_INCLUDED
