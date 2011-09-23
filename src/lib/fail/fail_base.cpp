#include <nyan/fail/fail_base.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

fail::fail(const source_coordinate &where_arg,
      const std::string &summary_arg) :
      my_record(where_arg, summary_arg)
{
   initialize();
}

fail::fail(const source_coordinate &where_arg,
      const char *summary_arg) :
      my_record(where_arg, summary_arg)
{
   initialize();
}

fail::~fail() throw()
{}

const char * fail::what() const throw()
{
   try
   {
      std::ostringstream msg;
      print_summary(msg);
      my_what_cache = msg.str();
      return my_what_cache.c_str();
   }
   catch (const std::exception &e)
   {
      std::ostringstream msg;
      msg << "i caught a secondary exception in fail::what(): " << e.what();
      my_what_cache = msg.str();
      return my_what_cache.c_str();
   }
}

void fail::initialize()
{
   my_record.type(this);
}

fail & fail::summary(const std::string &text_arg)
{
   my_record.summary(text_arg);
   return *this;
}

void fail::print_summary(std::ostream &out_arg) const
{
   return my_record.print_summary(out_arg);
}

fail & fail::where(const source_coordinate &where_arg)
{
   my_record.where(where_arg);
   return *this;
}

void fail::print_where(std::ostream &out_arg) const
{
   return my_record.print_where(out_arg);
}

void fail::print_type(std::ostream &out_arg) const
{
   return my_record.print_type(out_arg);
}

}
