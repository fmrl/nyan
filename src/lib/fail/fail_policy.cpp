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

#include <nyan/fail/fail_policy.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

void fail_policy::operator()(const std::exception &fail_arg) const
{
   std::cerr << "# an exception was unexpectedly thrown. std::exception::what() says, \""
         << fail_arg.what() << ".\"\n";
}

void fail_policy::operator()(const nyan::fail &fail_arg) const
{
#if NYAN_CAN_HAS_YAML
   YAML::Emitter y;
   y << fail_arg;
   if (y.good())
      std::cerr << y.c_str();
   else
   {
      std::cerr << "# an exception was unexpectedly thrown. std::exception::what() says, \""
            << fail_arg.what() << ".\"\n";
      std::cerr
         << "# unfortunately, i failed to provide details; the yaml emitter says, \""
         << y.GetLastError() << "\"\n";
   }
#else
   std::cerr << "# an exception was unexpectedly thrown. std::exception::what() says, \""
         << fail_arg.what()
         << ".\"\n# if you'd like more details, consider compiling libnyan with yaml support enabled.\n";
#endif
}

}