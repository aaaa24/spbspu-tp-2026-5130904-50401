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
  if (!(input >> size)) {
    return input;
  }

  dest.points.clear();
  dest.points.reserve(size);

  using iit_t = std::istream_iterator< Point >;
  std::copy_n(iit_t{input}, size, std::back_inserter(dest.points));

  if (dest.points.size() < size) {
    input.setstate(std::ios::failbit);
  }

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

chernov::IOguard::IOguard(std::basic_ios< char > & s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

chernov::IOguard::~IOguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
