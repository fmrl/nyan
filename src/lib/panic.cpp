#include <nyan/panic.hpp>

#include <stdexcept>
#include <cstdlib>
#include <iostream>

namespace
{
void print_prologue(std::ostream &out_arg,
      const nyan::source_coordinate &where_arg);

void print_prologue(std::ostream &out_arg,
      const nyan::source_coordinate &where_arg)
{
   out_arg << "the nyan::panic() stops at ";
   where_arg.print_friendly(out_arg);
   out_arg << " and says...\n\n";
}

}

namespace nyan
{

void panic(const char *msg_arg, const source_coordinate &where_arg)
{
   // i cannot use a panic_...() function here; it would cause a stack
   // overflow.
   if (NULL == msg_arg)
   {
      print_prologue(std::cerr, NYAN_HERE());
      std::cerr << "the NULL hits! the NULL hits! you die.";
      abort();
   }
   else
   {
      print_prologue(std::cerr, where_arg);
      std::cerr << msg_arg;
      abort();
   }
}

bool panic_if(bool condition_arg, const char *msg_arg,
      const source_coordinate &where_arg)
{
	if (condition_arg)
		panic(msg_arg, where_arg);
	return condition_arg;
}

const char * panic_if_empty(const char *str_arg,
      const source_coordinate &where_arg)
{
   NYAN_PANIC_IFNULL(str_arg);
   if ('\0' == str_arg[0])
   {
      panic("the emptiness hits! the emptiness hits! you die.", where_arg);
   }
   return str_arg;
}

char * panic_if_empty(char *str_arg, const source_coordinate &where_arg)
{
   NYAN_PANIC_IFNULL(str_arg);
   (void)panic_if_empty(const_cast< const char * >(str_arg), where_arg);
   return str_arg;
}

void panic_unreachable(const source_coordinate &where_arg)
{
   panic("the unreachable code hits! the unreachable code hits! you die. ",
         where_arg);
}

}
