#include "polygon.hpp"

#include <algorithm>
#include <iterator>
#include <limits>

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

  if (size < 3) {
    input.setstate(std::ios::failbit);
    return input;
  }

  std::vector< Point > points;
  points.reserve(size);

  using iit_t = std::istream_iterator< Point >;
  std::copy_n(iit_t{input}, size, std::back_inserter(points));
  input >> EndlIO{};

  if (dest.points.size() < size) {
    input.setstate(std::ios::failbit);
  }

  if (input) {
    dest.points = std::move(points);
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

std::istream & chernov::operator>>(std::istream & input, EndlIO &&)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  char c = 0;
  input >> c;
  if (input && c != '\n') {
    input.setstate(std::ios::failbit);
  }

  return input;
}

void chernov::inputPolygons(std::istream & input, std::vector< Polygon > & polygons)
{
  Polygon p;
  input >> p;

  if (input.eof()) {
    return;
  }

  if (!input.fail()) {
    polygons.push_back(p);
  } else {
    input.clear();
    input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  inputPolygons(input, polygons);
}
