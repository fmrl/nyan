#ifndef NYAN_ZERO_FAIL_HPP_IS_INCLUDED
#define NYAN_ZERO_FAIL_HPP_IS_INCLUDED

#include <nyan/fail/fail_base.hpp>

namespace nyan
{

class zero_fail :
   public fail
{
private:

   static const std::string our_identifier_field;
   static const std::string our_summary;

public:

   zero_fail(const source_coordinate &where_arg,
         const std::string &identifier_arg);
   zero_fail(const source_coordinate &where_arg,
         const char *identifier_arg);

   virtual ~zero_fail() throw();

   template < class T >
   static bool check(T ptr_arg)
   {
      return 0 == ptr_arg;
   }

private:

   void initialize(const char *identifier_arg);

};

}


#endif /* NYAN_ZERO_FAIL_HPP_IS_INCLUDED */
