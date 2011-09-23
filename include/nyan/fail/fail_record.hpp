#ifndef NYAN_FAIL_RECORD_HPP_IS_INCLUDED
#define NYAN_FAIL_RECORD_HPP_IS_INCLUDED

#include <nyan/source_coordinate.hpp>
#include <boost/regex.hpp>
#include <string>
#include <sstream>
#include <map>
#include <typeinfo>

namespace nyan
{

class fail_record
{
private:

   typedef std::map< std::string, std::string > fields_type;
   typedef boost::regex regex_type;
   typedef boost::smatch match_type;

   static const regex_type our_subst_re;

   fields_type my_fields;

public:

   static const std::string our_type_field;
   static const std::string our_summary_field;
   static const std::string our_where_field;

   fail_record(const source_coordinate &where_arg,
         const std::string &summary_arg);
   fail_record(const source_coordinate &where_arg,
         const char *summary_arg);

   virtual ~fail_record() throw();

   template < class Value >
   void sto(const std::string &name_arg, const Value &value_arg)
   {
      std::ostringstream s;
      s << value_arg;
      sto_string(name_arg, s.str());
   }

   void sto(const std::string &name_arg, const std::string &value_arg)
   {
      sto_string(name_arg, value_arg);
   }

   void sto(const std::string &name_arg, const char *value_arg)
   {
      sto_string(name_arg, value_arg);
   }

   void sto(const std::string &name_arg,
         const source_coordinate &where_arg)
   {
      sto_source_coordinate(name_arg, where_arg);
   }

   void sto_string(const std::string &name_arg,
         const std::string &value_arg);
   void sto_string(const std::string &name_arg,
         const char *value_arg);
   void sto_source_coordinate(const std::string &name_arg,
         const source_coordinate &where_arg);
   template < class Object >
   void sto_typename(const std::string &name_arg, const Object *ptr_arg)
   {
      return sto_string(name_arg, typeid(ptr_arg).name());
   }

   const std::string & rcl(const std::string &name_arg) const;
   void print(std::ostream &out_arg,
         const std::string &name_arg) const;

   void summary(const std::string &text_arg);
   const std::string & summary() const;
   void print_summary(std::ostream &out_arg) const;

   void where(const source_coordinate &where_arg);
   const std::string & where() const;
   void print_where(std::ostream &out_arg) const;

   template < class Object >
   void type(const Object *ptr_arg)
   {
      return sto_typename(our_type_field, ptr_arg);
   }
   const std::string & type() const;
   void print_type(std::ostream &out_arg) const;

private:

   void initialize(const source_coordinate &where_arg,
         const char *what_arg);

};

}

#endif // NYAN_FAIL_RECORD_HPP_IS_INCLUDED
