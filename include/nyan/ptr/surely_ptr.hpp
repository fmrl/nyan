#ifndef SURELY_PTR_HPP_IS_INCLUDED
#define SURELY_PTR_HPP_IS_INCLUDED

#include <nyan/ptr/ptr_base.hpp>

namespace nyan
{

template < class Element >
class surely_ptr :
   protected ptr_base
{

public:

   typedef Element element_type;

private:

   Element *my_ptr;

public:

   surely_ptr(Element * const ptr_arg) throw() :
      my_ptr(ptr_arg)
   {
      throw_if_null(ptr_arg);
      ptr_arg->incref();
   }

   surely_ptr(const surely_ptr &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {
      my_ptr->incref();
   }

   template < class Other >
   surely_ptr(const surely_ptr< Other > &other_arg) throw()  :
      my_ptr(other_arg.my_ptr)
   {
      my_ptr->incref();
   }

   ~surely_ptr() throw()
   {
      my_ptr->decref();
      this->~ptr_base();
   }

   surely_ptr & operator=(const surely_ptr &other_arg) throw()
   {
      unchecked_reset(other_arg.my_ptr);
      return *this;
   }

   template < class Other >
   surely_ptr & operator=(const surely_ptr< Other > &other_arg) throw()
   {
      unchecked_reset(other_arg.my_ptr);
      return *this;
   }

   surely_ptr & operator=(Element * const ptr_arg) throw()
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

   void swap(const surely_ptr &other_arg) throw()
   {
      std::swap(my_ptr, other_arg.my_ptr);
   }

private:

   surely_ptr() throw();

   void unchecked_reset(Element * const ptr_arg) throw()
   {
      ptr_arg->incref();
      my_ptr->decref();
      my_ptr = ptr_arg;
   }

};

template < class Element, class Other >
bool operator==(const surely_ptr< Element > &lhs_arg,
      const surely_ptr< Other > & rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element, class Other >
bool operator!=(const surely_ptr< Element > &lhs_arg,
      const surely_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg.get();
}

template < class Element >
bool operator==(const surely_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() == rhs_arg;
}

template < class Element >
bool operator!=(const surely_ptr< Element > &lhs_arg,
      const Element * const rhs_arg) throw()
{
   return lhs_arg.get() != rhs_arg;
}

template < class Element >
bool operator==(const Element * const lhs_arg,
      surely_ptr<Element> const & rhs_arg) throw()
{
   return rhs_arg == lhs_arg;
}

template < class Element >
bool operator!=(const Element * const lhs_arg,
      const surely_ptr< Element > &rhs_arg) throw()
{
   return rhs_arg != lhs_arg;
}

template< class Element, class Other >
bool operator<(const surely_ptr< Element > &lhs_arg,
      const surely_ptr< Other > &rhs_arg) throw()
{
   return lhs_arg.get() < rhs_arg.get();
}

template < class Element >
void swap(surely_ptr< Element > &lhs_arg,
      surely_ptr< Element > &rhs_arg) throw()
{
   lhs_arg.swap(rhs_arg);
}

template < class Char, class Traits, class Element >
std::basic_ostream < Char, Traits > &
   operator<<(std::basic_ostream< Char, Traits > &out_arg,
         const surely_ptr< Element > &ptr_arg)
{
   out_arg << ptr_arg.get();
}

}

#endif /* SURELY_PTR_HPP_IS_INCLUDED */
