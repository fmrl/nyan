#ifndef PTR_BASE_HPP_IS_INCLUDED
#define PTR_BASE_HPP_IS_INCLUDED

namespace nyan
{

class ptr_base
{
protected:

   ptr_base()
   {}

public:

   ~ptr_base()
   {}

protected:

   static void throw_if_null(const void * const ptr_arg);

};

}

#endif /* PTR_BASE_HPP_IS_INCLUDED */
