#include <nyan/fail/fail_record.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

// match index 0 will hold the entire string matched.
// match index 1 will refer to "\$", if detected.
// match index 2 will refer to "${varname}", if detected.
// if match index 2 is not empty, position 3 will hold "varname".
const fail_record::regex_type
   fail_record::our_subst_re("(\\\\\\$)|(\\$\\{([^$]+)\\})");

const std::string fail_record::our_type_field("type");
const std::string fail_record::our_summary_field("summary");
const std::string fail_record::our_where_field("where");

fail_record::fail_record(const source_coordinate &where_arg,
      const std::string &summary_arg)
{
   initialize(where_arg, summary_arg.c_str());
}

fail_record::fail_record(const source_coordinate &where_arg,
      const char *summary_arg)
{
   initialize(where_arg, summary_arg);
}

fail_record::~fail_record() throw()
{}

void fail_record::sto_string(const std::string &name_arg,
      const std::string &value_arg)
{
   NYAN_FAIL_IFEMPTY(name_arg);
   NYAN_FAIL_IFEMPTY(value_arg);

   my_fields[name_arg] = value_arg;
}

void fail_record::sto_string(const std::string &name_arg,
      const char *value_arg)
{
   NYAN_FAIL_IFEMPTY(name_arg);
   NYAN_FAIL_IFEMPTY(value_arg);

   my_fields[name_arg] = value_arg;
}

const std::string & fail_record::rcl(const std::string &name_arg) const
{
   fields_type::const_iterator i = my_fields.find(name_arg);
   if (my_fields.end() == i)
      throw not_found_fail(NYAN_HERE(), name_arg);
   else
      return i->second;
}

void fail_record::print(std::ostream &out_arg,
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
         out_arg << *s;
      }
   }

   out_arg << expr->substr(j, expr->npos);
}

void fail_record::sto_source_coordinate(const std::string &name_arg,
      const source_coordinate &where_arg)
{
   // [todo] i would like to eliminate string building such as this by
   // implementing a recursive string macro evaluator, which shouldn't be
   // too difficult. i would just need to add code that detects when a
   // specific macro has been expanded recursively. this seems fairly low
   // priority, however, given that i'm going to be generating backtraces
   // for each exception constructed.
   std::ostringstream txt;
   where_arg.print_friendly(txt);
   return sto(name_arg, txt.str());
}

void fail_record::initialize(const source_coordinate &where_arg,
      const char *summary_arg)
{
   NYAN_FAIL_IFNULL(summary_arg);

   where(where_arg);
   summary(summary_arg);
}

void fail_record::summary(const std::string &text_arg)
{
   sto(our_summary_field, text_arg);
}

const std::string & fail_record::summary() const
{
   return rcl(our_summary_field);
}

void fail_record::print_summary(std::ostream &out_arg) const
{
   print(out_arg, our_summary_field);
}

void fail_record::where(const source_coordinate &where_arg)
{
   sto(our_where_field, where_arg);
}

const std::string & fail_record::where() const
{
   return rcl(our_where_field);
}

void fail_record::print_where(std::ostream &out_arg) const
{
   print(out_arg, our_where_field);
}

const std::string & fail_record::type() const
{
   return rcl(our_type_field);
}

void fail_record::print_type(std::ostream &out_arg) const
{
   print(out_arg, our_type_field);
}

}
