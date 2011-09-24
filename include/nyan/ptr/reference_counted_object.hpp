#ifndef NYAN_REFERENCE_COUNTED_OBJECT_HPP_IS_INCLUDED
#define NYAN_REFERENCE_COUNTED_OBJECT_HPP_IS_INCLUDED

#include <nyan/atomic_integer.hpp>

namespace nyan
{

template < class RefCnt >
class basic_reference_counted_object
{

private:
   typedef basic_reference_counted_object< RefCnt > this_type;
public:
   typedef RefCnt refcnt_type;

private:
   mutable refcnt_type my_refcnt;

protected:

	basic_reference_counted_object()
	{}

public:

	virtual ~basic_reference_counted_object()
	{}

	void incref() const throw()
	{
		++my_refcnt;
	}

	void decref() const throw()
	{
	   // this is one of the few places i've found a necessity to use
	   // const_cast<>(). this makes it possible for const_..._ptr<> objects
	   // to function. technically, a reference count has no bearing on the
	   // "const-ness" of an object. it's should be an underlying,
	   // transparent mechanism.
		if (0 == --my_refcnt)
			const_cast< this_type * >(this)->finalize();
	}

protected:

	virtual void finalize() throw()
	{
		delete this;
	}

};

typedef basic_reference_counted_object< atomic_integer >
   reference_counted_object;

} // namespace nyan

#endif // NYAN_REFERENCE_COUNTED_OBJECT_HPP_IS_INCLUDED
