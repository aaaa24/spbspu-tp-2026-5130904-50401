#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "datastruct.hpp"

int main()
{
  std::vector< chernov::DataStruct > data;
  using iit_t = std::istream_iterator< chernov::DataStruct >;
  using oit_t = std::ostream_iterator< chernov::DataStruct >;

  std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
}
