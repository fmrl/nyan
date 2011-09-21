#ifndef NYAN_PANIC_HPP_IS_INCLUDED
#define NYAN_PANIC_HPP_IS_INCLUDED

#include <nyan/source_coordinate.hpp>

#define NYAN_PANIC_NOW(Message) \
      (::nyan::panic((Message), NYAN_HERE()));
#define NYAN_PANIC_IFZERO(Value) \
      (::nyan::panic_if_zero((Value), NYAN_HERE()));
#define NYAN_PANIC_IFNULL(Ptr) \
      (::nyan::panic_if_null((Ptr), NYAN_HERE()));
#define NYAN_PANIC_IFEMPTY(Container) \
      (::nyan::panic_if_empty((Container), NYAN_HERE()));
#define NYAN_PANIC_UNREACHABLE() \
      (::nyan::panic_unreachable(NYAN_HERE()));

namespace nyan
{

void panic(const char *msg_arg);
void panic(const char *msg_arg, const source_coordinate &where_arg);
bool panic_if(bool condition_arg, const char *msg_arg,
      const source_coordinate &where_arg);

template < class Lhs, class Rhs >
Lhs panic_if_equal(Lhs lhs_arg, Rhs rhs_arg, const char *msg_arg,
      const source_coordinate &where_arg)
{
	panic_if(lhs_arg == rhs_arg, msg_arg, where_arg);
	return lhs_arg;
}

template < class Value >
Value panic_if_zero(Value nz_arg, const source_coordinate &where_arg)
{
	panic_if_equal(static_cast< Value >(0), nz_arg,
		"the zero hits! the zero hits! you die.", where_arg);
	return nz_arg;
}

template < class Value >
const Value * panic_if_null(const Value *ptr_arg,
      const source_coordinate &where_arg)
{
   panic_if_equal(static_cast< const Value * >(0), ptr_arg,
      "the NULL hits! the NULL hits! you die.", where_arg);
   return ptr_arg;
}

template < class Value >
Value * panic_if_null(Value *ptr_arg,
      const source_coordinate &where_arg)
{
   (void)panic_if_null(const_cast< const Value * >(ptr_arg), where_arg);
   return ptr_arg;
}

template < class Something >
const Something & panic_if_empty(const Something &something_arg,
      const source_coordinate &where_arg)
{
   if (something_arg.empty())
   {
      panic("the emptiness hits! the emptiness hits! you die.", where_arg);
   }
   return something_arg;
}

const char * panic_if_empty(const char *str_arg,
      const source_coordinate &where_arg);

template < class Something >
Something & panic_if_empty(Something &something_arg,
      const source_coordinate &where_arg)
{
   (void)panic_if_empty(const_cast< const Something & >(something_arg),
         where_arg);
   return something_arg;
}

char * panic_if_empty(char *str_arg, const source_coordinate &where_arg);

void panic_unreachable(const source_coordinate &where_arg);

}

#endif // NYAN_PANIC_HPP_IS_INCLUDED
