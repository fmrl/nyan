#include <nyan/fail/zero_fail.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

const std::string zero_fail::our_identifier_field("identifier");
const std::string zero_fail::our_summary(
         "i unexpectedly encountered a variable (${identifier}) "
         "that contains a disallowed value (0) at ${where}.");

zero_fail::zero_fail(const source_coordinate &where_arg,
      const std::string &identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg.c_str());
}

zero_fail::zero_fail(const source_coordinate &where_arg,
      const char *identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg);
}

zero_fail::~zero_fail() throw()
{}

void zero_fail::initialize(const char *identifier_arg)
{
   NYAN_FAIL_IFEMPTY(identifier_arg);

   sto(our_identifier_field, identifier_arg);
}

}
