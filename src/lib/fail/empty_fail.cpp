#include <nyan/fail/empty_fail.hpp>

#include <nyan/fail.hpp>

namespace nyan
{
const std::string empty_fail::our_identifier_field("identifier");
const std::string empty_fail::our_summary(
      "i unexpectedly encountered an empty collection (${identifier}) "
      "at ${where}.");

empty_fail::empty_fail(const source_coordinate &where_arg,
      const std::string &identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg.c_str());
}

empty_fail::empty_fail(const source_coordinate &where_arg,
      const char *identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg);
}

empty_fail::~empty_fail() throw()
{}

void empty_fail::initialize(const char *identifier_arg)
{
   NYAN_FAIL_IFEMPTY(identifier_arg);

   sto(our_identifier_field, identifier_arg);
}

bool empty_fail::check(const char *str_arg)
{
   NYAN_FAIL_IFNULL(str_arg);
   return '\0' == str_arg[0];
}


}
