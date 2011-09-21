#include <nyan/source_coordinate.hpp>
#include <nyan/fail.hpp>

namespace nyan
{

source_coordinate::source_coordinate(const char *filen_arg,
      size_t lineno_arg) :
   my_filen(filen_arg),
   my_lineno(lineno_arg)
{
   NYAN_FAIL_IFEMPTY(filen_arg);
   // [todo] NYAN_FAIL_IFZERO(lineno_arg);
}

void source_coordinate::print_friendly(std::ostream &out_arg) const
{
   out_arg << "line " << my_lineno << " of \"" << my_filen << "\"";
}

}
