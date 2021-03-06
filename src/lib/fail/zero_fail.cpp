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

#include <nyan/fail/zero_fail.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

const std::string zero_fail::our_identifier_field_name("identifier");
const std::string zero_fail::our_summary(
         "i encountered a variable (${identifier}) "
         "that contains a disallowed value (0) at ${where}.");

zero_fail::zero_fail(const text_coordinate &where_arg,
      const std::string &identifier_arg) :
   fail(where_arg, our_summary)
{
   initialize(identifier_arg);
}

zero_fail::~zero_fail() throw()
{}

void zero_fail::initialize(const std::string &identifier_arg)
{
   insert(type(typeid(*this)));
   insert(identifier(identifier_arg));
}

const zero_fail::field
      zero_fail::identifier(const std::string &identifier_arg)
{
   return field(our_identifier_field_name, identifier_arg);
}

}

// $vim:23: vim:set sts=3 sw=3 et:,$
