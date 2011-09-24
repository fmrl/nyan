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

#include <nyan/panic.hpp>

#include <stdexcept>
#include <cstdlib>
#include <iostream>

namespace
{
void print_prologue(std::ostream &out_arg,
      const nyan::source_coordinate &where_arg);

void print_prologue(std::ostream &out_arg,
      const nyan::source_coordinate &where_arg)
{
   out_arg << "the nyan::panic() stops at ";
   where_arg.print_friendly(out_arg);
   out_arg << " and says...\n\n";
}

}

namespace nyan
{

void panic(const char *msg_arg, const source_coordinate &where_arg)
{
   // i cannot use a panic_...() function here; it would cause a stack
   // overflow.
   if (NULL == msg_arg)
   {
      print_prologue(std::cerr, NYAN_HERE());
      std::cerr << "the NULL hits! the NULL hits! you die.";
      abort();
   }
   else
   {
      print_prologue(std::cerr, where_arg);
      std::cerr << msg_arg;
      abort();
   }
}

bool panic_if(bool condition_arg, const char *msg_arg,
      const source_coordinate &where_arg)
{
	if (condition_arg)
		panic(msg_arg, where_arg);
	return condition_arg;
}

const char * panic_if_empty(const char *str_arg,
      const source_coordinate &where_arg)
{
   NYAN_PANIC_IFNULL(str_arg);
   if ('\0' == str_arg[0])
   {
      panic("the emptiness hits! the emptiness hits! you die.", where_arg);
   }
   return str_arg;
}

char * panic_if_empty(char *str_arg, const source_coordinate &where_arg)
{
   NYAN_PANIC_IFNULL(str_arg);
   (void)panic_if_empty(const_cast< const char * >(str_arg), where_arg);
   return str_arg;
}

void panic_unreachable(const source_coordinate &where_arg)
{
   panic("the unreachable code hits! the unreachable code hits! you die. ",
         where_arg);
}

}
