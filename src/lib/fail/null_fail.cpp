#include <nyan/fail/null_fail.hpp>

namespace nyan
{

const std::string null_fail::our_variable_name_field("variable_name");
const std::string null_fail::our_summary(
         "i unexpectedly encountered a pointer variable (${variable_name}) "
         "that contains a NULL value at ${where}.");

null_fail::null_fail(const source_coordinate &where_arg,
      const std::string &variable_arg) :
   fail(where_arg, our_summary)
{
   initialize(variable_arg.c_str());
}

null_fail::null_fail(const source_coordinate &where_arg,
      const char *variable_arg) :
   fail(where_arg, our_summary)
{
   initialize(variable_arg);
}

null_fail::~null_fail() throw()
{}

void null_fail::initialize(const char *variable_arg)
{
   // a panic is necessary here to avoid infinite recursion.
   NYAN_PANIC_IFEMPTY(variable_arg);

   sto(our_variable_name_field, variable_arg);
}

}
