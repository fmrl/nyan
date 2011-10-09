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

#ifndef NYAN_FAIL_POLICY_HPP_IS_INCLUDED
#define NYAN_FAIL_POLICY_HPP_IS_INCLUDED

// acknowledgement: i figured how to use boost::mpl thanks to the generous
// advice provided in a discussion at <http://groups.google.com/group/boost-list/browse_thread/thread/1d879d7512d7f968?pli=1>.

#include <nyan/fail/fail_base.hpp>

#include <nyan/config.h>
#if NYAN_CAN_HAS_YAML
#     include <yaml-cpp/yaml.h>
#endif // NYAN_CAN_HAS_YAMLCPP

#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/reverse.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include <exception>

namespace nyan
{

namespace detail
{

template < class It, class End, class Policy, class To >
bool apply_fail_policy3(boost::mpl::true_, Policy, To to_arg)
{
   to_arg();
   return false;
}

template < class It, class End, class Policy, class To >
bool apply_fail_policy3(boost::mpl::false_, Policy policy_arg, To to_arg)
{
   typedef typename boost::mpl::deref< It >::type Exception;
   typedef typename boost::mpl::next< It >::type Next;

   try
   {
      return apply_fail_policy3< Next, End >(
            typename boost::is_same< Next, End >::type(),
            policy_arg, to_arg);
   }
   catch (const Exception &e)
   {
      policy_arg(e);
      return true;
   }
}

template < class Protocol, class Policy, class To >
bool apply_fail_policy2(Policy policy_arg, To to_arg)
{
   typedef typename boost::mpl::begin< Protocol >::type Begin;
   typedef typename boost::mpl::end< Protocol >::type End;
   return apply_fail_policy3< Begin, End >(
         typename boost::is_same< Begin, End >::type(),
         policy_arg, to_arg);
}

} //namespace detail

template < class Policy, class To >
bool apply_fail_policy(Policy policy_arg, To to_arg)
{
   return detail::apply_fail_policy2<
         typename boost::mpl::reverse< typename Policy::protocol >::type
         >(policy_arg, to_arg);
}

class fail_policy
{
public:

   typedef boost::mpl::vector< nyan::fail, std::exception > protocol;

   void operator()(const std::exception &fail_arg) const;
   void operator()(const nyan::fail &fail_arg) const;

};

}

#endif /* NYAN_FAIL_POLICY_HPP_IS_INCLUDED */
