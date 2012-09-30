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

#ifndef NYAN_ERRNO_FAIL_HPP_IS_INCLUDED
#define NYAN_ERRNO_FAIL_HPP_IS_INCLUDED

#include <nyan/fail/fail_base.hpp>

#include <cerrno>

namespace nyan
{

class errno_fail :
   public fail
{
private:

   static const std::string our_function_name;
   static const std::string our_errno_name;
   static const std::string our_strerror_name;
   static const std::string our_summary;

public:

   errno_fail(const nyan::text_coordinate &where_arg,
         const std::string &funcn_arg, int errno_arg);

   errno_fail(const nyan::text_coordinate &where_arg,
         const std::string &funcn_arg);

   virtual ~errno_fail() throw();

private:

   static const field function(const std::string &name_arg);
   static const field errno_value(int errno_arg);
   static const field strerror(int errno_arg);

   void initialize(const std::string &funcn_arg, int errno_arg);

};

}//namespace nyan

#endif // NYAN_ERRNO_FAIL_HPP_IS_INCLUDED

// $vim:23: vim:set sts=3 sw=3 et:,$
