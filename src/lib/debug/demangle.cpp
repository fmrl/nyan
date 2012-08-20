// $legal:1626:
// 
// Copyright (c) 2011, Michael Lowell Roberts.  
// All rights reserved. 
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are 
// met: 
// 
//   - Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer. 
// 
//   - Redistributions in binary form must reproduce the above copyright 
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the distribution.
//  
//   - Neither the name of the copyright holder nor the names of 
//   contributors may be used to endorse or promote products derived 
//   from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER 
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
// 
// ,$

#include <nyan/debug/demangle.hpp>

#include <nyan/panic.hpp>

#include <cassert>
#include <cstdio>       // required for the demangling code (sscanf).
#include <cstdlib>
#include <cxxabi.h>     // required to demangle C++ function names.
#include <sstream>

namespace nyan
{

demangle::demangle()
{}

demangle::~demangle()
{}

bool demangle::operator()(std::string &result_arg, const char *symbol_arg)
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
      print_status(txt, status);
      result_arg = txt.str();
   }

   if (NULL != s)
      free(s);

   return 0 == status;
}

void demangle::print_status(std::ostream &out_arg, int status_arg)
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

}

// $vim:23: vim:set sts=3 sw=3 et:,$
