#ifndef NYAN_EMPTY_FAIL_HPP_IS_INCLUDED
#define NYAN_EMPTY_FAIL_HPP_IS_INCLUDED

#include <nyan/fail/fail_base.hpp>
#include <nyan/fail/null_fail.hpp>
#include <nyan/fail/fail_meta.hpp>

namespace nyan
{

class empty_fail :
   public fail
{
private:

   static const std::string our_variable_name_field;
   static const std::string our_what_message;

public:

   empty_fail(const source_coordinate &where_arg,
         const std::string &variable_arg);
   empty_fail(const source_coordinate &where_arg,
         const char *variable_arg);

   virtual ~empty_fail() throw();

   template < class T >
   static bool check(const T &container_arg)
   {
      return container_arg.empty();
   }

   static bool check(const char *str_arg);

private:

   void initialize(const char *variable_arg);

};

}

#endif /* NYAN_EMPTY_FAIL_HPP_IS_INCLUDED */
