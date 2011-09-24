#ifndef NYAN_FAIL_META_HPP_IS_INCLUDED
#define NYAN_FAIL_META_HPP_IS_INCLUDED

#include <nyan/source_coordinate.hpp>

// i use the do...while() pattern here instead of putting the if statement
// into the inline function because i can avoid invoking NYAN_HERE() in the
// most likely path. this also avoids stack overflows in
// source_coordinate::source_coordinate().
#define NYAN_FAIL_IFEMPTY(Container) \
   do \
   { \
      if (::nyan::empty_fail::check(Container)) \
         throw ::nyan::empty_fail(NYAN_HERE(), #Container); \
   } \
   while (0)

#define NYAN_FAIL_IFNULL(Ptr) \
   do \
   { \
      if (::nyan::null_fail::check(Ptr)) \
         throw ::nyan::null_fail(NYAN_HERE(), #Ptr); \
   } \
   while (0)

#define NYAN_FAIL_IFZERO(Value) \
   do \
   { \
      if (::nyan::zero_fail::check(Value)) \
         throw ::nyan::zero_fail(NYAN_HERE(), #Value); \
   } \
   while (0)

#endif // NYAN_FAIL_META_HPP_IS_INCLUDED
