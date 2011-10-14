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

#ifndef NYAN_CONVERT_NUMBER_HPP_IS_INCLUDED
#define NYAN_CONVERT_NUMBER_HPP_IS_INCLUDED

#include <boost/mpl/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/type_traits/is_same.hpp>

namespace nyan
{

template< class To, class From >
void convert_number(To &to_arg, From from_arg)
{
   // i can't see a reason why the return type shouldn't match the To
   // template parameter. i'm putting a meta-assertion in just in case
   // i'm missing something.
   typedef typename boost::numeric::converter< To, From >::result_type
         result_type;
   BOOST_MPL_ASSERT((boost::is_same< To, result_type >));

   to_arg = boost::numeric::converter< To, From >::convert(from_arg);
}

}

#endif /* NYAN_CONVERT_NUMBER_HPP_IS_INCLUDED */
