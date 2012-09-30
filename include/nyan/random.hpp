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

#include <boost/random/random_device.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread/tss.hpp>

namespace nyan
{

namespace details
{

template < class Value, class Engine >
void default_pop_random(Value &result_arg, Engine &engine_arg)
{
   // [mlr][todo] this should be a nyan exception.
   throw std::logic_error("unimplemented");
}

template < class Engine >
void pop_uniform_random(int &result_arg, Engine &engine_arg)
{
   engine_arg.pop(result_arg,
         boost::random::uniform_int_distribution< int >());
}

template < class Engine >
void pop_uniform_random(int &result_arg, Engine &engine_arg, int min_arg,
      int max_arg)
{
   engine_arg.pop(result_arg,
         boost::random::uniform_int_distribution< int >(min_arg, max_arg));
}

}// namespace details

// [mlr][todo] i would like to put Distribution and Engine into random_traits.
template < class Engine >
class basic_random
{
public:

   typedef Engine engine_type;
   typedef typename engine_type::result_type seed_type;

   private: typedef basic_random< Engine > this_type;
   private: typedef boost::thread_specific_ptr< this_type > tss_type;

   private: static tss_type our_tss;

   private: engine_type my_engine;

public:

   // [mlr][todo] what about a seed?
   basic_random()
   {
      seed(generate_seed());
   }

   basic_random(seed_type seed_arg)
   {
      seed(seed_arg);
   }

   static seed_type generate_seed()
   {
      typedef boost::random::random_device rng_type;
      rng_type g;
      boost::random::uniform_int_distribution< rng_type::result_type > d;
      rng_type::result_type n = d(g);
      // [mlr][todo] numeric conversion!
      return seed_type(n);
   }

   virtual ~basic_random()
   {}

   void seed(seed_type seed_arg)
   {
      my_engine.seed(seed_arg);
   }

   seed_type seed() const
   {
      return my_engine.seed();
   }

   template < class Value, class Filter >
   void pop(Value &result_arg, const Filter &filter_arg)
   {
      result_arg = filter_arg(my_engine);
   }

   template < class Value >
   void pop(Value &result_arg)
   {
      details::pop_uniform_random(result_arg, my_engine);
   }

   template < class Value >
   void pop(Value &result_arg, Value min_arg, Value max_arg)
   {
      details::pop_uniform_random(result_arg, my_engine, min_arg,
            max_arg);
   }

private:

   static void local(this_type *&result_arg)
   {
      result_arg = NULL;
      this_type *p = our_tss.get();
      if (NULL == p)
         p = initialize_tss();
      result_arg = p;
   }

   static void local(const this_type *&result_arg)
   {
      this_type *p;
      local(p);
      result_arg = p;
   }

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

template < class Engine >
typename basic_random< Engine >::tss_type
   basic_random< Engine >::our_tss;

typedef basic_random< boost::random::mt19937 > random;

}// namespace nyan

#endif //!defined(NYAN_RANDOM_HPP_IS_INCLUDED)

// $vim:23: vim:set sts=3 sw=3 et:,$
