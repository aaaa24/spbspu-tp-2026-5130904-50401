#include "polygon.hpp"

#include <algorithm>
#include <iterator>

std::istream & chernov::operator>>(std::istream & input, Point & dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  using sep = DelimiterIO;
  input >> sep{'('} >> dest.x >> sep{';'} >> dest.y >> sep{')'};
  return input;
}

std::istream & chernov::operator>>(std::istream & input, Polygon & dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  size_t size = 0;
  input >> size;

  using iit_t = std::istream_iterator< Point >;
  std::copy_n(iit_t{input}, size, std::back_inserter(dest.points));
  return input;
}

std::istream & chernov::operator>>(std::istream & input, DelimiterIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  char c = 0;
  input >> c;
  if (input && c != dest.exp) {
    input.setstate(std::ios::failbit);
  }
  return input;
}
