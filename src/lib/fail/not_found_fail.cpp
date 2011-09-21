#include <nyan/fail/not_found_fail.hpp>

#include <nyan/fail/empty_fail.hpp>
#include <nyan/fail/fail_meta.hpp>

namespace nyan
{

const std::string not_found_fail::our_search_term_attribute("search_term");
const std::string not_found_fail::our_summary(
      "my search for \"${search_term}\" failed unexpectedly at ${where}.");

not_found_fail::not_found_fail(const source_coordinate &where_arg,
      const std::string &searchterm_arg) :
   fail(where_arg, our_summary)
{
   initialize(searchterm_arg.c_str());
}

not_found_fail::not_found_fail(const source_coordinate &where_arg,
      const char *searchterm_arg) :
   fail(where_arg, our_summary)
{
   initialize(searchterm_arg);
}

not_found_fail::~not_found_fail() throw()
{}

void not_found_fail::initialize(const char *searchterm_arg)
{
   NYAN_FAIL_IFEMPTY(searchterm_arg);

   sto(our_search_term_attribute, searchterm_arg);
}

}

