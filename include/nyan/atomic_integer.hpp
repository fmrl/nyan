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

#ifndef NYAN_ATOMIC_INTEGER_HPP_IS_INCLUDED
#define NYAN_ATOMIC_INTEGER_HPP_IS_INCLUDED

#include <boost/thread/thread.hpp>

namespace nyan
{

// the interface on this class is constrained by what can be performed
// with lockless operations on platforms that support them.
template < class Value >
class basic_atomic_integer
{
private:

	typedef basic_atomic_integer< Value > this_type;

public:

	typedef Value value_type;

private:

	typedef boost::mutex mutex_type;

	volatile value_type my_value;
	mutex_type my_mutex;

public:

	basic_atomic_integer()
	{
		initialize(0);
	}

	basic_atomic_integer(value_type src_arg)
	{
		initialize(src_arg);
	}

	~basic_atomic_integer()
	{}

	bool assign_if_equal(value_type src_arg, value_type cmp_arg)
	{
		mutex_type::scoped_lock mine(my_mutex);
		if (cmp_arg == my_value)
		{
			my_value = src_arg;
			return true;
		}
		else
			return false;
	}

	value_type operator++()
	{
		mutex_type::scoped_lock mine(my_mutex);
		return ++my_value;
	}

	value_type operator--()
	{
		mutex_type::scoped_lock mine(my_mutex);
		return --my_value;
	}

	template < class Other >
	bool operator==(Other other_arg) const
	{
		return my_value == other_arg;
	}

private:

	basic_atomic_integer(const this_type &);

	void initialize(value_type rhs_arg)
	{
		my_value = rhs_arg;
	}

};

template < class Lhs, class Rhs >
bool operator==(Lhs lhs, const basic_atomic_integer< Rhs > &rhs)
{
	// i can avoid a friend declaration if i just take advantage of
   // communitive property of equality.
	return rhs == lhs;
}

// lockless specializations.
// TODO: specialize the entire class to eliminate the mutex, which is
// unnecessary when lockless arithmetic is used.
#if defined(_WIN32)

#include <Windows.h>

template < >
LONG basic_atomic_integer< LONG >::operator++()
{
	return InterlockedIncrement(&my_value);
}

template < >
LONG basic_atomic_integer< LONG >::operator--()
{
	return InterlockedDecrement(&my_value);
}

template < >
bool basic_atomic_integer< LONG >::assign_if_equal(value_type src_arg,
      value_type cmp_arg)
{
	// InterlockedCompareExchange() returns what 'my_value' was before the
	// operation. If it was equal to 'cmp' before the operation, then the
	// assignment must have occurred.
	return InterlockedCompareExchange(&my_value, src, cmp) == cmp;
}

typedef basic_atomic_integer< LONG > atomic_integer;

#else

typedef basic_atomic_integer< int > atomic_integer;

#endif //defined(_WIN32)

}//namespace nyan

#endif // NYAN_ATOMIC_INTEGER_HPP_IS_INCLUDED

// $vim:23: vim:set sts=3 sw=3 et:,$
