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
