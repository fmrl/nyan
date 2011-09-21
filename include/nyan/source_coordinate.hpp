#ifndef NYAN_SOURCE_COORDINATE_HPP_IS_INCLUDED
#define NYAN_SOURCE_COORDINATE_HPP_IS_INCLUDED

#include <string>

namespace nyan
{

class source_coordinate
{
private:

   const char *my_filen;
   size_t my_lineno;

public:

   source_coordinate(const char *filen_arg, size_t lineno_arg);

   // i eliminated the virtual dispatch because this class is intended to
   // be constructed on the stack with frequency.
   ~source_coordinate()
   {}

   const char * filen() const
   {
      return my_filen;
   }

   size_t lineno() const
   {
      return my_lineno;
   }

   void print_friendly(std::ostream &out_arg) const;

};

}

#define NYAN_HERE() (::nyan::source_coordinate(__FILE__, __LINE__))

#endif /* NYAN_SOURCE_COORDINATE_HPP_IS_INCLUDED */
