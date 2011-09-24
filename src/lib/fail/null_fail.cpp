#include <nyan/fail/null_fail.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

const std::string null_fail::our_identifier_field("identifier");
const std::string null_fail::our_summary(
         "i unexpectedly encountered a pointer variable (${identifier}) "
         "that contains a NULL value at ${where}.");

null_fail::null_fail(const source_coordinate &where_arg,
      const std::string &identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg.c_str());
}

null_fail::null_fail(const source_coordinate &where_arg,
      const char *identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg);
}

null_fail::~null_fail() throw()
{}

void null_fail::initialize(const char *identifier_arg)
{
   NYAN_FAIL_IFEMPTY(identifier_arg);

   sto(our_identifier_field, identifier_arg);
}

}
