#include <nyan/ptr/ptr_base.hpp>

#include <nyan/fail.hpp>

namespace nyan
{

void ptr_base::throw_if_null(const void * const ptr_arg)
{
   NYAN_FAIL_IFNULL(ptr_arg);
}

}
