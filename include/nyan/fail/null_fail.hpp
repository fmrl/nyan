#ifndef NYAN_NULL_FAIL_HPP_IS_INCLUDED
#define NYAN_NULL_FAIL_HPP_IS_INCLUDED

#include <nyan/fail/fail_base.hpp>
#include <nyan/panic.hpp>

namespace nyan
{

class null_fail :
   public fail
{
private:

   static const std::string our_variable_name_attribute;
   static const std::string our_summary;

public:

   null_fail(const source_coordinate &where_arg,
         const std::string &variable_arg);
   null_fail(const source_coordinate &where_arg,
         const char *variable_arg);

   virtual ~null_fail() throw();

   template < class T >
   static bool check(const T *ptr_arg)
   {
      return NULL == ptr_arg;
   }

private:

   void initialize(const char *variable_arg);

};

}


#endif /* NYAN_NULL_FAIL_HPP_IS_INCLUDED */
