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

#ifndef CONST_TRUSTED_PTR_HPP_IS_INCLUDED
#define CONST_TRUSTED_PTR_HPP_IS_INCLUDED

#include <nyan/ptr/trusted_ptr.hpp>

namespace nyan
{

template < class Element >
class const_trusted_ptr
{

public:

   typedef Element element_type;

private:

   typedef trusted_ptr< const Element > my_ptr_type;

   my_ptr_type my_ptr;

public:

   const_trusted_ptr(const Element * const ptr_arg) throw() :
      my_ptr(ptr_arg)
   {}

   const_trusted_ptr(const const_trusted_ptr &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   template < class Other >
   const_trusted_ptr(const const_trusted_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   template < class Other >
   const_trusted_ptr(const trusted_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.get())
   {}

   ~const_trusted_ptr() throw()
   {}

   const_trusted_ptr & operator=(const const_trusted_ptr &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   template < class Other >
   const_trusted_ptr & operator=(const const_trusted_ptr< Other > &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   template < class Other >
   const_trusted_ptr & operator=(const trusted_ptr< Other > &other_arg) throw()
   {
      my_ptr = other_arg.get();
      return *this;
   }

   const_trusted_ptr & operator=(const Element * const ptr_arg) throw()
   {
      my_ptr = ptr_arg;
      return *this;
   }

   void reset(const Element * const ptr_arg) throw()
   {
      my_ptr.reset(ptr_arg);
   }

   const Element & operator*() const
   {
      return my_ptr.operator*();
   }

   const Element * operator->() const
   {
      return my_ptr.operator->();
   }

   const Element * get() const throw()
   {
      return my_ptr.get();
   }

   void swap(const const_trusted_ptr &other_arg) throw()
   {
      my_ptr.swap(other_arg.my_ptr);
   }

private:

   const_trusted_ptr() throw();

};

template < class Element, class Other >
bool operator==(const const_trusted_ptr< Element > &lhs_arg,
      const const_trusted_ptr< Other > & rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element, class Other >
bool operator!=(const const_trusted_ptr< Element > &lhs_arg,
      const const_trusted_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element >
bool operator==(const const_trusted_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg;
}

template < class Element >
bool operator!=(const const_trusted_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() != rhs_arg;
}

template < class Element >
bool operator==(const Element * const lhs_arg,
      const_trusted_ptr<Element> const & rhs_arg) throw()
{
   return rhs_arg == lhs_arg;
}

template < class Element >
bool operator!=(const Element * const lhs_arg,
      const const_trusted_ptr< Element > &rhs_arg) throw()
{
   return rhs_arg != lhs_arg;
}

template< class Element, class Other >
bool operator<(const const_trusted_ptr< Element > &lhs_arg,
      const const_trusted_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() < rhs_arg.get();
}

template < class Element >
void swap(const_trusted_ptr< Element > &lhs_arg,
      const_trusted_ptr< Element > &rhs_arg) throw()
{
   lhs_arg.swap(rhs_arg);
}

template < class Char, class Traits, class Element >
std::basic_ostream < Char, Traits > &
   operator<<(std::basic_ostream< Char, Traits > &out_arg,
         const const_trusted_ptr< Element > &ptr_arg)
{
   out_arg << ptr_arg.get();
}

}

#endif /* CONST_TRUSTED_PTR_HPP_IS_INCLUDED */
