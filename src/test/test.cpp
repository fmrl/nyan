#include <nyan/fail.hpp>

#include <iostream>

int main()
{
   try
   {
      int *x = NULL;

      NYAN_FAIL_IFNULL(x);
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what();
   }

   return 0;
}
