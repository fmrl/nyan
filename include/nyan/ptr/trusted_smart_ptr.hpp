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

#ifndef NYAN_TRUSTED_SMART_PTR_HPP_IS_INCLUDED
#define NYAN_TRUSTED_SMART_PTR_HPP_IS_INCLUDED

#include <nyan/destroy.hpp>
#include <nyan/ptr/ptr_base.hpp>

namespace nyan
{

template < class Element >
class trusted_smart_ptr :
   protected ptr_base
{

public:

   typedef Element element_type;

private:

   Element *my_ptr;

public:

   trusted_smart_ptr(Element * const ptr_arg) throw() :
      my_ptr(ptr_arg)
   {
      throw_if_null(ptr_arg);
      ptr_arg->incref();
   }

   trusted_smart_ptr(const trusted_smart_ptr &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {
      my_ptr->incref();
   }

   template < class Other >
   trusted_smart_ptr(const trusted_smart_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {
      my_ptr->incref();
   }

   ~trusted_smart_ptr() throw()
   {
      my_ptr->decref();
      nyan::invoke_destructor(static_cast< ptr_base & >(*this));
   }

   trusted_smart_ptr & operator=(const trusted_smart_ptr &other_arg) throw()
   {
      unchecked_reset(other_arg.my_ptr);
      return *this;
   }

   template < class Other >
   trusted_smart_ptr & operator=(const trusted_smart_ptr< Other > &other_arg) throw()
   {
      unchecked_reset(other_arg.my_ptr);
      return *this;
   }

   trusted_smart_ptr & operator=(Element * const ptr_arg) throw()
   {
      reset(ptr_arg);
      return *this;
   }

   void reset(Element * const ptr_arg) throw()
   {
      throw_if_null(ptr_arg);
      unchecked_reset(ptr_arg);
   }

   Element & operator*() const
   {
      return *my_ptr;
   }

   Element * operator->() const
   {
      return my_ptr;
   }

   Element * get() const throw()
   {
      return my_ptr;
   }

   void swap(const trusted_smart_ptr &other_arg) throw()
   {
      std::swap(my_ptr, other_arg.my_ptr);
   }

private:

   trusted_smart_ptr() throw();

   void unchecked_reset(Element * const ptr_arg) throw()
   {
      ptr_arg->incref();
      my_ptr->decref();
      my_ptr = ptr_arg;
   }

};

template < class Element, class Other >
bool operator==(const trusted_smart_ptr< Element > &lhs_arg,
      const trusted_smart_ptr< Other > & rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element, class Other >
bool operator!=(const trusted_smart_ptr< Element > &lhs_arg,
      const trusted_smart_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element >
bool operator==(const trusted_smart_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg;
}

template < class Element >
bool operator!=(const trusted_smart_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() != rhs_arg;
}

template < class Element >
bool operator==(const Element * const lhs_arg,
      trusted_smart_ptr<Element> const & rhs_arg) throw()
{
   return rhs_arg == lhs_arg;
}

template < class Element >
bool operator!=(const Element * const lhs_arg,
      const trusted_smart_ptr< Element > &rhs_arg) throw()
{
   return rhs_arg != lhs_arg;
}

template< class Element, class Other >
bool operator<(const trusted_smart_ptr< Element > &lhs_arg,
      const trusted_smart_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() < rhs_arg.get();
}

template < class Element >
void swap(trusted_smart_ptr< Element > &lhs_arg,
      trusted_smart_ptr< Element > &rhs_arg) throw()
{
   lhs_arg.swap(rhs_arg);
}

template < class Char, class Traits, class Element >
std::basic_ostream < Char, Traits > &
   operator<<(std::basic_ostream< Char, Traits > &out_arg,
         const trusted_smart_ptr< Element > &ptr_arg)
{
   out_arg << ptr_arg.get();
}

}

#endif /* NYAN_TRUSTED_SMART_PTR_HPP_IS_INCLUDED */
