#ifndef NYAN_FAIL_BASE_HPP_IS_INCLUDED
#define NYAN_FAIL_BASE_HPP_IS_INCLUDED

#include <nyan/source_coordinate.hpp>
#include <nyan/fail/fail_record.hpp>
#include <string>
#include <exception>
#include <map>

namespace nyan
{

class fail :
   public std::exception
{
private:

   fail_record my_record;
   mutable std::string my_what_cache;

protected:

   fail(const source_coordinate &where_arg,
         const std::string &summary_arg);
   fail(const source_coordinate &where_arg,
         const char *summary_arg);

public:

   virtual ~fail() throw();

   virtual const char * what() const throw();

   const fail_record & record() const
   {
      return my_record;
   }

   fail_record & record()
   {
      return my_record;
   }

   template < class Value >
   fail & sto(const std::string &name_arg, const Value &value_arg)
   {
      my_record.sto(name_arg, value_arg);
      return *this;
   }

   void print(std::ostream &out_arg,
         const std::string &name_arg) const;

   fail & summary(const std::string &text_arg);
   void print_summary(std::ostream &out_arg) const;
   fail & where(const source_coordinate &where_arg);
   void print_where(std::ostream &out_arg) const;
   void print_type(std::ostream &out_arg) const;

private:

   void initialize();

};

}

#endif // NYAN_FAIL_BASE_HPP_IS_INCLUDED
