#ifndef CONST_SURELY_PTR_HPP_IS_INCLUDED
#define CONST_SURELY_PTR_HPP_IS_INCLUDED

#include <nyan/ptr/surely_ptr.hpp>

namespace nyan
{

template < class Element >
class const_surely_ptr
{

public:

   typedef Element element_type;

private:

   typedef surely_ptr< const Element > my_ptr_type;

   my_ptr_type my_ptr;

public:

   const_surely_ptr(const Element * const ptr_arg) throw() :
      my_ptr(ptr_arg)
   {}

   const_surely_ptr(const const_surely_ptr &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   template < class Other >
   const_surely_ptr(const const_surely_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {}

   template < class Other >
   const_surely_ptr(const surely_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.get())
   {}

   ~const_surely_ptr() throw()
   {}

   const_surely_ptr & operator=(const const_surely_ptr &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   template < class Other >
   const_surely_ptr & operator=(const const_surely_ptr< Other > &other_arg) throw()
   {
      my_ptr = other_arg.my_ptr;
      return *this;
   }

   template < class Other >
   const_surely_ptr & operator=(const surely_ptr< Other > &other_arg) throw()
   {
      my_ptr = other_arg.get();
      return *this;
   }

   const_surely_ptr & operator=(const Element * const ptr_arg) throw()
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

   void swap(const const_surely_ptr &other_arg) throw()
   {
      my_ptr.swap(other_arg.my_ptr);
   }

private:

   const_surely_ptr() throw();

};

template < class Element, class Other >
bool operator==(const const_surely_ptr< Element > &lhs_arg,
      const const_surely_ptr< Other > & rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element, class Other >
bool operator!=(const const_surely_ptr< Element > &lhs_arg,
      const const_surely_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element >
bool operator==(const const_surely_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg;
}

template < class Element >
bool operator!=(const const_surely_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() != rhs_arg;
}

template < class Element >
bool operator==(const Element * const lhs_arg,
      const_surely_ptr<Element> const & rhs_arg) throw()
{
   return rhs_arg == lhs_arg;
}

template < class Element >
bool operator!=(const Element * const lhs_arg,
      const const_surely_ptr< Element > &rhs_arg) throw()
{
   return rhs_arg != lhs_arg;
}

template< class Element, class Other >
bool operator<(const const_surely_ptr< Element > &lhs_arg,
      const const_surely_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() < rhs_arg.get();
}

template < class Element >
void swap(const_surely_ptr< Element > &lhs_arg,
      const_surely_ptr< Element > &rhs_arg) throw()
{
   lhs_arg.swap(rhs_arg);
}

template < class Char, class Traits, class Element >
std::basic_ostream < Char, Traits > &
   operator<<(std::basic_ostream< Char, Traits > &out_arg,
         const const_surely_ptr< Element > &ptr_arg)
{
   out_arg << ptr_arg.get();
}

}

#endif /* CONST_SURELY_PTR_HPP_IS_INCLUDED */
