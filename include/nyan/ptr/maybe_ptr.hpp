#ifndef MAYBE_PTR_HPP_IS_INCLUDED
#define MAYBE_PTR_HPP_IS_INCLUDED

#include <nyan/ptr/ptr_base.hpp>
#include <boost/intrusive_ptr.hpp>
#include <cassert>

namespace nyan
{

template < class Element >
class maybe_ptr :
   protected ptr_base
{

public:

   typedef Element element_type;

private:

   typedef boost::intrusive_ptr< Element > my_ptr_type;

   my_ptr_type my_ptr;

public:

   maybe_ptr() throw()
   {}

   maybe_ptr(Element * const ptr_arg) throw() :
      my_ptr(ptr_arg)
   {}

   maybe_ptr(const maybe_ptr &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   template < class Other >
   maybe_ptr(const maybe_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   ~maybe_ptr() throw()
   {
      this->~ptr_base();
   }

   maybe_ptr & operator=(const maybe_ptr &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   template < class Other >
   maybe_ptr & operator=(const maybe_ptr< Other > &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   maybe_ptr & operator=(Element * const ptr_arg) throw()
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

   void swap(const maybe_ptr &other_arg) throw()
   {
      my_ptr.swap(other_arg.my_ptr);
   }

};

template < class Element, class Other >
bool operator==(const maybe_ptr< Element > &lhs_arg,
      const maybe_ptr< Other > & rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element, class Other >
bool operator!=(const maybe_ptr< Element > &lhs_arg,
      const maybe_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element >
bool operator==(const maybe_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg;
}

template < class Element >
bool operator!=(const maybe_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() != rhs_arg;
}

template < class Element >
bool operator==(const Element * const lhs_arg,
      maybe_ptr<Element> const & rhs_arg) throw()
{
   return rhs_arg == lhs_arg;
}

template < class Element >
bool operator!=(const Element * const lhs_arg,
      const maybe_ptr< Element > &rhs_arg) throw()
{
   return rhs_arg != lhs_arg;
}

template< class Element, class Other >
bool operator<(const maybe_ptr< Element > &lhs_arg,
      const maybe_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() < rhs_arg.get();
}

template < class Element >
void swap(maybe_ptr< Element > &lhs_arg,
      maybe_ptr< Element > &rhs_arg) throw()
{
   lhs_arg.swap(rhs_arg);
}

template < class Char, class Traits, class Element >
std::basic_ostream < Char, Traits > &
   operator<<(std::basic_ostream< Char, Traits > &out_arg,
         const maybe_ptr< Element > &ptr_arg)
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

#endif /* MAYBE_PTR_HPP_IS_INCLUDED */
