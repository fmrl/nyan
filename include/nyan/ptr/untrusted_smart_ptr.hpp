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

#ifndef NYAN_UNTRUSTED_SMART_PTR_HPP_IS_INCLUDED
#define NYAN_UNTRUSTED_SMART_PTR_HPP_IS_INCLUDED

#include <nyan/functions/destroy.hpp>
#include <nyan/ptr/ptr_base.hpp>

#include <boost/intrusive_ptr.hpp>

#include <cassert>

namespace nyan
{

template < class Element >
class untrusted_smart_ptr :
   protected ptr_base
{

public:

   typedef Element element_type;

private:

   typedef boost::intrusive_ptr< Element > my_ptr_type;

   my_ptr_type my_ptr;

public:

   untrusted_smart_ptr() throw()
   {}

   untrusted_smart_ptr(Element * const ptr_arg) throw() :
      my_ptr(ptr_arg)
   {}

   untrusted_smart_ptr(const untrusted_smart_ptr &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   template < class Other >
   untrusted_smart_ptr(const untrusted_smart_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   ~untrusted_smart_ptr() throw()
   {
      nyan::invoke_destructor(static_cast< ptr_base & >(*this));
   }

   untrusted_smart_ptr & operator=(const untrusted_smart_ptr &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   template < class Other >
   untrusted_smart_ptr & operator=(const untrusted_smart_ptr< Other > &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   untrusted_smart_ptr & operator=(Element * const ptr_arg) throw()
   {
      my_ptr = ptr_arg;
      return *this;
   }

   void reset() throw()
   {
      my_ptr.reset();
   }

   void reset(Element * const ptr_arg) throw()
   {
      my_ptr.reset(ptr_arg);
   }

   Element & operator*() const
   {
      throw_if_null(my_ptr.get());
      return my_ptr.operator*();
   }

   Element * operator->() const
   {
      throw_if_null(my_ptr.get());
      return my_ptr.operator->();
   }

   Element * get() const throw()
   {
      return my_ptr.get();
   }

   bool empty() const throw()
   {
      return NULL == my_ptr.get();
   }

   operator bool() const throw()
   {
      return empty();
   }

   void swap(const untrusted_smart_ptr &other_arg) throw()
   {
      my_ptr.swap(other_arg.my_ptr);
   }

};

template < class Element, class Other >
bool operator==(const untrusted_smart_ptr< Element > &lhs_arg,
      const untrusted_smart_ptr< Other > & rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element, class Other >
bool operator!=(const untrusted_smart_ptr< Element > &lhs_arg,
      const untrusted_smart_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element >
bool operator==(const untrusted_smart_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg;
}

template < class Element >
bool operator!=(const untrusted_smart_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() != rhs_arg;
}

template < class Element >
bool operator==(const Element * const lhs_arg,
      untrusted_smart_ptr<Element> const & rhs_arg) throw()
{
   return rhs_arg == lhs_arg;
}

template < class Element >
bool operator!=(const Element * const lhs_arg,
      const untrusted_smart_ptr< Element > &rhs_arg) throw()
{
   return rhs_arg != lhs_arg;
}

template< class Element, class Other >
bool operator<(const untrusted_smart_ptr< Element > &lhs_arg,
      const untrusted_smart_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() < rhs_arg.get();
}

template < class Element >
void swap(untrusted_smart_ptr< Element > &lhs_arg,
      untrusted_smart_ptr< Element > &rhs_arg) throw()
{
   lhs_arg.swap(rhs_arg);
}

template < class Char, class Traits, class Element >
std::basic_ostream < Char, Traits > &
   operator<<(std::basic_ostream< Char, Traits > &out_arg,
         const untrusted_smart_ptr< Element > &ptr_arg)
{
   out_arg << ptr_arg.get();
}

template < class Value >
void intrusive_ptr_add_ref(Value * const ptr_arg)
{
   assert(ptr_arg != NULL);

   ptr_arg->incref();
}

template < class Value >
void intrusive_ptr_release(Value * const ptr_arg)
{
   assert(ptr_arg != NULL);

   ptr_arg->decref();
}

}

#endif /* NYAN_UNTRUSTED_SMART_PTR_HPP_IS_INCLUDED */

// $vim:23: vim:set sts=3 sw=3 et:,$
