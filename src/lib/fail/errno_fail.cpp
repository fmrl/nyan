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

#define STRERROR_BUFLEN 256

#include <nyan/fail/errno_fail.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

const std::string errno_fail::our_function_name("function");
const std::string errno_fail::our_errno_name("errno");
const std::string errno_fail::our_strerror_name("strerror");
const std::string errno_fail::our_summary(
         "i encountered an error (${strerror}) while using the "
            "${function} function.");

errno_fail::errno_fail(const text_coordinate &where_arg,
      const std::string &funcn_arg, int errno_arg) :
   fail(where_arg, our_summary)
{
   // [mlr][todo] can't this be done in initialize()?
   insert(type(typeid(*this)));
   initialize(funcn_arg, errno_arg);
}
errno_fail::errno_fail(const text_coordinate &where_arg,
      const std::string &funcn_arg) :
   fail(where_arg, our_summary)
{
   // [mlr][todo] can't this be done in initialize()?
   insert(type(typeid(*this)));
   initialize(funcn_arg, errno);
}

errno_fail::~errno_fail() throw()
{}

void errno_fail::initialize(const std::string &funcn_arg, int errno_arg)
{
   insert(function(funcn_arg));
   insert(errno_value(errno_arg));
   insert(strerror(errno_arg));
}

const errno_fail::field
      errno_fail::function(const std::string &name_arg)
{
   return field(our_function_name, name_arg);
}

const errno_fail::field
      errno_fail::errno_value(int errno_arg)
{
   return field(our_errno_name, errno_arg);
}

const errno_fail::field
      errno_fail::strerror(int errno_arg)
{
   char s[STRERROR_BUFLEN + 1];
   // [mlr] we expect the XSI compliant version of strerror_r().
   if (0 == strerror_r(errno_arg, s, STRERROR_BUFLEN))
      return field(our_strerror_name, s);
   else
      return field(our_strerror_name, "*strerror_r() failed*");
}

}

// $vim:23: vim:set sts=3 sw=3 et:,$
