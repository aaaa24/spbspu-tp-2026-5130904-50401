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

  struct CounterIO {
    size_t size_;
    size_t count_;

    CounterIO(size_t size):
      size_(size),
      count_(0)
    {}

    bool operator()(const Point &)
    {
      return count_++ < size_;
    }
  };

  auto counter = CounterIO{size};

  using iit_t = std::istream_iterator< Point >;
  std::copy_if(iit_t{input}, iit_t{}, std::back_inserter(dest.points), counter);
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
