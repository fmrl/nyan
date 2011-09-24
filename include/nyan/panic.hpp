#/* ex: set softtabstop=3 shiftwidth=3 expandtab: */

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

#ifndef NYAN_PANIC_HPP_IS_INCLUDED
#define NYAN_PANIC_HPP_IS_INCLUDED

#include <nyan/source_coordinate.hpp>

#define NYAN_PANIC_NOW(Message) \
      (::nyan::panic((Message), NYAN_HERE()));
#define NYAN_PANIC_IFZERO(Value) \
      (::nyan::panic_if_zero((Value), NYAN_HERE()));
#define NYAN_PANIC_IFNULL(Ptr) \
      (::nyan::panic_if_null((Ptr), NYAN_HERE()));
#define NYAN_PANIC_IFEMPTY(Container) \
      (::nyan::panic_if_empty((Container), NYAN_HERE()));
#define NYAN_PANIC_UNREACHABLE() \
      (::nyan::panic_unreachable(NYAN_HERE()));

namespace nyan
{

void panic(const char *msg_arg);
void panic(const char *msg_arg, const source_coordinate &where_arg);
bool panic_if(bool condition_arg, const char *msg_arg,
      const source_coordinate &where_arg);

template < class Lhs, class Rhs >
Lhs panic_if_equal(Lhs lhs_arg, Rhs rhs_arg, const char *msg_arg,
      const source_coordinate &where_arg)
{
	panic_if(lhs_arg == rhs_arg, msg_arg, where_arg);
	return lhs_arg;
}

template < class Value >
Value panic_if_zero(Value nz_arg, const source_coordinate &where_arg)
{
	panic_if_equal(static_cast< Value >(0), nz_arg,
		"the zero hits! the zero hits! you die.", where_arg);
	return nz_arg;
}

template < class Value >
const Value * panic_if_null(const Value *ptr_arg,
      const source_coordinate &where_arg)
{
   panic_if_equal(static_cast< const Value * >(0), ptr_arg,
      "the NULL hits! the NULL hits! you die.", where_arg);
   return ptr_arg;
}

template < class Value >
Value * panic_if_null(Value *ptr_arg,
      const source_coordinate &where_arg)
{
   (void)panic_if_null(const_cast< const Value * >(ptr_arg), where_arg);
   return ptr_arg;
}

template < class Something >
const Something & panic_if_empty(const Something &something_arg,
      const source_coordinate &where_arg)
{
   if (something_arg.empty())
   {
      panic("the emptiness hits! the emptiness hits! you die.", where_arg);
   }
   return something_arg;
}

const char * panic_if_empty(const char *str_arg,
      const source_coordinate &where_arg);

template < class Something >
Something & panic_if_empty(Something &something_arg,
      const source_coordinate &where_arg)
{
   (void)panic_if_empty(const_cast< const Something & >(something_arg),
         where_arg);
   return something_arg;
}

char * panic_if_empty(char *str_arg, const source_coordinate &where_arg);

void panic_unreachable(const source_coordinate &where_arg);

}

#endif // NYAN_PANIC_HPP_IS_INCLUDED
