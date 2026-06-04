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

  using sep = detail::DelimiterIO;
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
  if (!(input >> size) || size < 3) {
    input.setstate(std::ios::failbit);
    dest.points.clear();
    return input;
  }

  std::vector< Point > temp;
  temp.reserve(size);

  if (!detail::readPoints(input, temp, size)) {
    input.setstate(std::ios::failbit);
    dest.points.clear();
    return input;
  }

  dest.points = std::move(temp);
  return input;
}

std::istream & chernov::detail::operator>>(std::istream & input, DelimiterIO && dest)
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

void chernov::inputPolygons(std::istream & input, std::vector< Polygon > & polygons)
{
  if (input.eof()) {
    return;
  }

  Polygon p;
  input >> p;

  if (!input.fail() && !p.points.empty()) {
    polygons.push_back(std::move(p));
  } else if (input.fail()) {
    input.clear();
    input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  if (!input.eof()) {
    inputPolygons(input, polygons);
  }
}

bool chernov::detail::readPoints(std::istream & input, std::vector< Point > & points, size_t left)
{
  if (left == 0) {
    return isLineEnd(input);
  }

  if (isLineEnd(input)) {
    return false;
  }

  Point p{};
  if (!(input >> p)) {
    return false;
  }

  points.push_back(p);
  return readPoints(input, points, left - 1);
}

void chernov::detail::skipSpaces(std::istream & input)
{
  int ch = input.peek();
  if (ch == ' ' || ch == '\t' || ch == '\r') {
    input.get();
    skipSpaces(input);
  }
}

bool chernov::detail::isLineEnd(std::istream & input)
{
  skipSpaces(input);
  int ch = input.peek();
  return ch == '\n' || ch == EOF;
}
