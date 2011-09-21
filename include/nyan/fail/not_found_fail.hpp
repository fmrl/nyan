#ifndef NYAN_NOT_FOUND_FAIL_HPP_IS_INCLUDED
#define NYAN_NOT_FOUND_FAIL_HPP_IS_INCLUDED

#include <nyan/fail/fail_base.hpp>
#include <nyan/panic.hpp>

namespace nyan
{

class not_found_fail :
   public fail
{
private:

   static const std::string our_search_term_attribute;
   static const std::string our_summary;

public:

   not_found_fail(const source_coordinate &where_arg,
         const std::string &searchterm_arg);
   not_found_fail(const source_coordinate &where_arg,
         const char *searchterm_arg);

   virtual ~not_found_fail() throw();

private:

   void initialize(const char *searchterm_arg);

};

}

#endif /* NYAN_NOT_FOUND_FAIL_HPP_IS_INCLUDED */
