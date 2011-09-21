#ifndef NYAN_FAIL_BASE_HPP_IS_INCLUDED
#define NYAN_FAIL_BASE_HPP_IS_INCLUDED

#include <nyan/source_coordinate.hpp>
#include <boost/regex.hpp>
#include <string>
#include <sstream>
#include <exception>
#include <map>

namespace nyan
{

class fail :
   public std::exception
{
private:

   typedef std::map< std::string, std::string > attribs_type;
   typedef boost::regex regex_type;
   typedef boost::smatch match_type;

   static const regex_type our_subst_re;

   attribs_type my_attribs;
   mutable std::string my_what_cache;

public:

   static const std::string our_type_attribute;
   static const std::string our_summary_attribute;
   static const std::string our_where_attribute;

protected:

   fail(const source_coordinate &where_arg,
         const std::string &summary_arg);
   fail(const source_coordinate &where_arg,
         const char *summary_arg);

public:

   virtual ~fail() throw();

   virtual const char * what() const throw();

   template < class Value >
   fail & sto(const std::string &name_arg, const Value &value_arg)
   {
      std::ostringstream s;
      s << value_arg;
      return sto_string(name_arg, s.str());
   }

   fail & sto(const std::string &name_arg, const std::string &value_arg)
   {
      return sto_string(name_arg, value_arg);
   }

   fail & sto_type(const std::string &typen_arg);
   fail & sto_where(const source_coordinate &where_arg);
   fail & sto_summary(const std::string &text_arg);

private:

   fail & sto_type();
   fail & sto_string(const std::string &name_arg,
         const std::string &value_arg);

public:
   const std::string & rcl(const std::string &name_arg) const;
   void print(std::ostream &out_arg,
         const std::string &name_arg) const;

private:

   void initialize(const source_coordinate &where_arg,
         const char *what_arg);

};

}

#endif // NYAN_FAIL_BASE_HPP_IS_INCLUDED
