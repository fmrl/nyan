#include <nyan/fail.hpp>

#include <iostream>

int main()
{
   try
   {
      int x = 0;

      NYAN_FAIL_IFZERO(x);
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what();
   }

   return 0;
}
