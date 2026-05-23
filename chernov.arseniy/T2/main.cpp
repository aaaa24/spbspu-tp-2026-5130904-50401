#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

#include "datastruct.hpp"

int main()
{
  std::vector< chernov::DataStruct > data;
  using iit_t = std::istream_iterator< chernov::DataStruct >;
  using oit_t = std::ostream_iterator< chernov::DataStruct >;
  std::istream & input = std::cin;
  std::ostream & output = std::cout;
  std::streamsize max_streamsize = std::numeric_limits< std::streamsize >::max();

  while (input) {
    std::copy(iit_t{input}, iit_t{}, std::back_inserter(data));
    if (!input.eof()) {
      input.clear();
      input.ignore(max_streamsize, '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), oit_t{output, "\n"});
}
