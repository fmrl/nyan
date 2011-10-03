/* ex: set softtabstop=3 shiftwidth=3 expandtab: */

/* This file is part of the *nyan* project at <http://fmrl.org>.
 * Copyright (c) 2011, Michael Lowell Roberts.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 *  - Neither the name of the copyright holder nor the names of
 *  contributors may be used to endorse or promote products derived
 *  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <nyan/debug/backtrace.hpp>

#include <nyan/panic.hpp>
#include <nyan/debug.hpp>

#include <cassert>
#include <cstdio>       // required for the backtrace code (sscanf).
#include <cstdlib>
#include <execinfo.h>   // required for access to backtrace() et al.

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
   demangle d;

   // this code was based on a tutorial about obtaining a backtrace at
   // <http://mykospark.net/2009/09/runtime-backtrace-in-c-with-name-demangling/>
   assert(symbol_arg != NULL);

   char temp[128];

   // first, try to decode_symbol a c++ name
   std::string demangled;
   if (1 == sscanf(symbol_arg, "%*[^(]%*[^_]%127[^)+]", temp))
   {
      if (d(demangled, temp))
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

std::ostream & operator<<(std::ostream &out_arg, const backtrace &what_arg)
{
   what_arg.print(out_arg);
   return out_arg;
}

}
