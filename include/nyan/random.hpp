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

#ifndef NYAN_RANDOM_HPP_IS_INCLUDED
#define NYAN_RANDOM_HPP_IS_INCLUDED

#include <boost/thread/tss.hpp>

namespace nyan
{

// [mlr][todo] i would like to put Distribution and Engine into random_traits.
template <
   typename Value,
   class Distribution = boost::uniform_int< Value >,
   class Engine = boost::mt19937 >
class basic_random
{
public:

   typedef Value value_type;
   typedef Engine engine_type;
   typedef Distribution distribution_type;

   // [mlr][todo] i would like to be able to pop more than one type of
   // value off of the generator.
   static void pop(value_type &result_arg)
   {
      this_type *p = our_tss.get();
      if (NULL == p)
         p = initialize_tss();
      result_arg = (*p->my_boost_rng)();
   }

private:

   typedef basic_random< Value, Distribution, Engine > this_type;
   typedef boost::thread_specific_ptr< this_type > tss_type;
   typedef boost::variate_generator< engine_type, distribution_type >
      boost_rng_type;

   static tss_type our_tss;

   engine_type my_engine;
   distribution_type my_distribution;
   std::auto_ptr< boost_rng_type > my_boost_rng;

public:

   // [mlr][todo] what about a seed?
   basic_random() :
      my_distribution(std::numeric_limits< value_type >::min(),
            std::numeric_limits< value_type >::max())
   {
      // TODO: seed the rng with a truly random value.
      // TODO: make the seed specifiable for reproducability in testing.
      // TODO: confirm the following:
      // i have to initialize the random generator /after/ the distribution
      // and i'm not guaranteed that if they're both in the initializer list.
      my_boost_rng.reset(new boost_rng_type(my_engine, my_distribution));
   }

   virtual ~basic_random()
   {}

private:

   static this_type * initialize_tss()
   {
      // this isn't meant to be called if something's already in our_tss.
      if (NULL == our_tss.get())
      {
         this_type * const p = new this_type;
         our_tss.reset(p);
         return p;
      }
      else
         // [mlr][todo] throw nyan error.
         throw std::logic_error("precondition fail");
   }

   static void destroy_ptr(void *ptr_arg)
   {
      this_type * const ob =
         reinterpret_cast< basic_random * >(ptr_arg);
      delete ob;
   }

};

template <typename Value, class Distribution, class Engine >
typename basic_random< Value, Distribution, Engine >::tss_type
   basic_random< Value, Distribution, Engine >::our_tss;

}// namespace nyan

#endif //!defined(NYAN_RANDOM_HPP_IS_INCLUDED)

// $vim:23: vim:set sts=3 sw=3 et:,$
