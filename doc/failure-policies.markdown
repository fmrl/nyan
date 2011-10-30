failure policies in libnyan
===========================

written by [michael lowell roberts][1].

overview
--------

in C++, exception handling provides a mechanism that makes error reporting and handling more convenient for the programmer than simple return codes allow. a catch block is a control structure, which unfortunately forces the programmer to resort to [copypasta][3] when describing common reactions to anticipated exceptions. this practice is unmanageable. furthermore, it discourages the placement of `try`...`catch` blocks where they would be appropriate.

*failure policies* are a means by which the programmer can define a vocabulary for how to react to an   exception anticipated in a given situation.

prerequisites
-------------

failure policies, as implemented in the [nyan][4] library, require [the boost metaprogramming library][5] to compile.

basic usage
-----------

a failure policy is represented as a class with the following members defined:

 - a *protocol declaration*.
 - a set of *response methods*.

the protocol declaration
------------------------

the `protocol` member is a typelist (`boost::mpl::vector`) that describes which exception types the policy understands. the types are expected to be listed in order from most specific to least specific.

for example, the following definition of `protocol` first considers exceptions of type `nyan::fail`, then of type `std::exception`:

	typedef boost::mpl::vector< nyan::fail, std::exception > protocol;

this is equivalent to specifying the following series of `catch` statements:

	try
	{
	   ...
	}
	catch (const nyan::fail &e)
	{
	   ...
	}
	catch (const std::exception &e)
	{
	   ...
	}

no equivalent to `catch (...)` is supported.

response methods
----------------

the failure policy class should include a response method for each type declared in the `protocol` typelist. each response method is represented with an overload of `operator()` that conforms to the following signature:

	void operator()(const T &fail_arg) const;

wherever the policy is applied to a function call, if exception *T* is caught, it is dispatched to the corresponding response method.

example policy
--------------

the following is an example of a failure policy declaration that understands how to respond to `nyan::fail` and `std::exception` exceptions:

	class fail_policy
	{
	public:

	   typedef boost::mpl::vector< nyan::fail, std::exception > protocol;

	   void operator()(const std::exception &fail_arg) const;
	   void operator()(const nyan::fail &fail_arg) const;

	};

application
-----------

failure policies, in the *nyan* library, are applied using the `nyan::apply_fail_policy()` method.

the following code will invoke the response method defined in the example associated with `std::exception`:

	#include <nyan/fail.hpp>
	#include <stdexcept>

	void foo(int x)
	{
	   if (0 == x)
	      throw std::runtime_error("oops.");
	}

	int main()
	{
	   if (nyan::apply_fail_policy(nyan::fail_policy(), foo, 0))
	      return 1;
	   else
	      return 0;
	}

`nyan::apply_fail_policy()` returns `true` if an exception was caught. this gives an enclosing loop the ability to terminate, should it see fit to do so.

the advantage of this approach is that a programmer can define a policy once and apply it throughout the source code simply by referring to it by name. the result is reasonably maintainable, since a change in policy propagates throughout the rest of the source code effortlessly. 

consider the case of a destructor, which is not permitted to throw exceptions:

	void object::finalize()
	{
	   // finalization code.
	}
	
	object::~object()
	{
	   (void)nyan::apply_fail_policy(destructor_fail_policy(),
	         std::mem_fun(&object::finalize), this);
	}

ordinarily, checking for anticipated exceptions within each destructor would require considerable copy-and-paste coding. most programmers wouldn't bother with such an endeavor. additionally, the code would not be adaptable to the needs of new situations without a continuous effort. this is a reasonable undertaking, however, when using the failure policy abstraction.

_____
**failure-policies.markdown**.  
copyright (c) 2011, [michael lowell roberts][1].  
licensed under the [creative commons attribution 3.0 unported license][2].  
all rights reserved. 
[1]:http://fmrl.org  
[2]:http://creativecommons.org/licenses/by/3.0/
[3]:http://knowyourmeme.com/memes/copypasta
[4]:http://github.com/fmrl/nyan
[5]:http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html


