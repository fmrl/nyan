#include <nyan/fail/empty_fail.hpp>

namespace nyan
{
const std::string empty_fail::our_variable_name_attribute("variable_name");
const std::string empty_fail::our_what_message(
      "i unexpectedly encountered an empty collection (${variable_name}) "
      "at ${where}.");

empty_fail::empty_fail(const source_coordinate &where_arg,
      const std::string &variable_arg) :
   fail(where_arg, our_what_message)
{
   initialize(variable_arg.c_str());
}

empty_fail::empty_fail(const source_coordinate &where_arg,
      const char *variable_arg) :
   fail(where_arg, our_what_message)
{
   initialize(variable_arg);
}

empty_fail::~empty_fail() throw()
{}

void empty_fail::initialize(const char *variable_arg)
{
   NYAN_FAIL_IFNULL(variable_arg);

   sto(our_variable_name_attribute, variable_arg);
}

bool empty_fail::check(const char *str_arg)
{
   NYAN_FAIL_IFNULL(str_arg);
   return '\0' == str_arg[0];
}


}
