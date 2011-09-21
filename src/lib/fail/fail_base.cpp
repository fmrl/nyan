#include <nyan/fail/fail_base.hpp>

#include <nyan/panic.hpp>
#include <nyan/fail/not_found_fail.hpp>
#include <typeinfo>

namespace nyan
{

// match index 0 will hold the entire string matched.
// match index 1 will refer to "\$", if detected.
// match index 2 will refer to "${varname}", if detected.
// if match index 2 is not empty, position 3 will hold "varname".
const fail::regex_type fail::our_subst_re("(\\\\\\$)|(\\$\\{([^$]+)\\})");

const std::string fail::our_type_attribute("type");
const std::string fail::our_summary_attribute("summary");
const std::string fail::our_where_attribute("where");

fail::fail(const source_coordinate &where_arg,
      const std::string &what_arg)
{
   initialize(where_arg, what_arg.c_str());
}

fail::fail(const source_coordinate &where_arg,
      const char *what_arg)
{
   initialize(where_arg, what_arg);
}

fail::~fail() throw()
{}

const char * fail::what() const throw()
{
   try
   {
      std::ostringstream msg;
      print(msg, our_summary_attribute);
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

fail & fail::sto_string(const std::string &name_arg,
      const std::string &value_arg)
{
   NYAN_PANIC_IFEMPTY(name_arg);
   NYAN_PANIC_IFEMPTY(value_arg);

   my_attribs[name_arg] = value_arg;

   return *this;
}

const std::string & fail::rcl(const std::string &name_arg) const
{
   attribs_type::const_iterator i = my_attribs.find(name_arg);
   if (my_attribs.end() == i)
      throw not_found_fail(NYAN_HERE(), name_arg);
   else
      return i->second;
}

fail & fail::sto_type(const std::string &typen_arg)
{
   NYAN_PANIC_IFEMPTY(typen_arg);

   return sto(our_type_attribute, typen_arg);
}

fail & fail::sto_type()
{
   // [todo] this is not thread-safe, iirc.
   return sto_type(typeid(this).name());
}

void fail::print(std::ostream &out_arg,
      const std::string &name_arg) const
{
   const std::string * const expr = &rcl(name_arg);

   boost::sregex_iterator end;
   boost::sregex_iterator i(expr->begin(), expr->end(), our_subst_re);
   // *j* will track the last character matched by the regex iterator.
   // this is important because we need to know how much to append to the
   // end of the output to make our task complete.
   std::string::size_type j = 0;
   for (; i != end; ++i)
   {
      out_arg << i->prefix();
      j += i->prefix().length() + (*i)[0].length();
      if ((*i)[1].matched)
         out_arg << "$";
      else
      {
         const std::string * const s = &rcl((*i)[3]);
         out_arg << s;
      }
   }

   out_arg << expr->substr(j, expr->npos);
}

fail & fail::sto_where(const source_coordinate &where_arg)
{
   // [todo] i would like to eliminate string building such as this by
   // implementing a recursive string macro evaluator, which shouldn't be
   // too difficult. i would just need to add code that detects when a
   // specific macro has been expanded recursively. this seems fairly low
   // priority, however, given that i'm going to be generating backtraces
   // for each exception constructed.
   std::ostringstream txt;
   where_arg.print_friendly(txt);
   return sto(our_where_attribute, txt.str());
}

void fail::initialize(const source_coordinate &where_arg,
      const char *summary_arg)
{
   NYAN_PANIC_IFNULL(summary_arg);

   sto_type();
   sto_where(where_arg);
   sto_summary(summary_arg);
}

fail & fail::sto_summary(const std::string &text_arg)
{
   return sto(our_summary_attribute, text_arg);
}

}
