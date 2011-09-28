#include <nyan/backtrace.hpp>

#include <nyan/panic.hpp>

#include <cassert>
#include <cstdio>       // required for the demangling code (sscanf).
#include <cstdlib>
#include <cxxabi.h>     // required to demangle C++ function names.
#include <execinfo.h>   // required for access to backtrace() et al.
#include <sstream>

namespace nyan
{

backtrace::backtrace()
{
   initialize(0);
}

backtrace::backtrace(int startat_arg)
{
   initialize(startat_arg);
}

backtrace::~backtrace()
{
   if (NULL != my_frames)
      free(my_frames);
}

void backtrace::print(std::ostream &out_arg) const
{
   for (size_t i = my_starting_frame; i < my_size; ++i)
   {
      decode_symbol(out_arg, my_frames[i]);
      out_arg << "\n";
   }
}

void backtrace::initialize(int startat_arg)
{
   // *startat_arg* is normalized so that frame 0 refers to the frame of
   // the caller, which is absolute frame 1. therefore, a value of -2
   // should show the complete backtrace.
   // [todo] i need to implement a set of numeric cast functions.
   my_starting_frame = std::max(0, startat_arg + 2);
   void *ptrs[our_max_frames];
   my_size = ::backtrace(ptrs, our_max_frames);
   my_frames = ::backtrace_symbols(ptrs, my_size);
}

void backtrace::decode_symbol(std::ostream &out_arg, const char *symbol_arg)
{
   // this code was based on a tutorial about obtaining a backtrace at
   // <http://mykospark.net/2009/09/runtime-backtrace-in-c-with-name-demangling/>
   assert(symbol_arg != NULL);

   char temp[128];

   // first, try to decode_symbol a c++ name
   std::string demangled;
   if (1 == sscanf(symbol_arg, "%*[^(]%*[^_]%127[^)+]", temp))
   {
      if (demangle(demangled, temp))
      {
         out_arg << demangled;
         return;
      }
   }
   // if that didn't work, try to get a regular c symbol
   if (1 == sscanf(symbol_arg, "%127s", temp))
   {
      out_arg << temp;
      return;
   }

   // if all else fails, just return the symbol and the demangle() status
   // code, if there was one.
   out_arg << symbol_arg;
   if (!demangled.empty())
      out_arg << "\t# demangle() says \"" << demangled << "\".\n";
}

bool backtrace::demangle(std::string &result_arg, const char *symbol_arg)
{
   NYAN_PANIC_IFEMPTY(symbol_arg);

   result_arg.clear();

   int status = 1;
   char * const s = abi::__cxa_demangle(symbol_arg, NULL, NULL, &status);
   if (0 == status)
      result_arg = s;
   else
   {
      std::ostringstream txt;
      print_demangle_status(txt, status);
      result_arg = txt.str();
   }

   if (NULL != s)
      free(s);

   return 0 == status;
}

void backtrace::print_demangle_status(std::ostream &out_arg, int status_arg)
{
   switch (status_arg)
   {
   default:
      out_arg << "an unexpected value (" << status_arg << ")";
      return;
   case 0:
      out_arg << "success";
      return;
   case -1:
      out_arg << "memory allocation failure";
      return;
   case -2:
      out_arg << "non-comforming name";
      return;
   case -3:
      out_arg << "invalid argument";
      return;
   }
}

std::ostream & operator<<(std::ostream &out_arg, const backtrace &what_arg)
{
   what_arg.print(out_arg);
   return out_arg;
}

}
