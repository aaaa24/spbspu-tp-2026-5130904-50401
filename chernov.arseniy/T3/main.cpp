#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "polygon.hpp"

int main(int argc, char ** argv)
{
  if (argc < 2) {
    std::cerr << "first argument must be name of file\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "could not open file\n";
    return 1;
  }

  std::vector< chernov::Polygon > polygons;
  using iit_t = std::istream_iterator< chernov::Polygon >;
  std::streamsize max_streamsize = std::numeric_limits< std::streamsize >::max();

  while (file) {
    std::copy(iit_t{file}, iit_t{}, std::back_inserter(polygons));
    if (!file.eof()) {
      file.clear();
      file.ignore(max_streamsize, '\n');
    }
  }
}
