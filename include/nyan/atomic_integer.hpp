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
