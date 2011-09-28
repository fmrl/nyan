#ifndef NYAN_BACKTRACE_HPP_IS_INCLUDED
#define NYAN_BACKTRACE_HPP_IS_INCLUDED

#include <iostream>
#include <memory>

namespace nyan
{

class backtrace
{
public:

   typedef size_t size_type;

private:

   enum { our_max_frames = 25 };

   char **my_frames;
   size_type my_size;
   size_type my_starting_frame;

public:

   backtrace();
   explicit backtrace(int startat_arg);

   virtual ~backtrace();

   void print(std::ostream &out_arg) const;

private:

   void initialize(int startat_arg);
   static void decode_symbol(std::ostream &out_arg, const char *symbol_arg);

   static bool demangle(std::string &result_arg, const char *symbol_arg);
   static void print_demangle_status(std::ostream &out_arg, int status_arg);

};

std::ostream & operator<<(std::ostream &out_arg, const backtrace &what_arg);

}

#endif /* NYAN_BACKTRACE_HPP_IS_INCLUDED */
