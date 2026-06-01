#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace chernov {
  struct Point {
    int x, y;
  };

  struct Polygon {
    std::vector< Point > points;
  };

  struct DelimiterIO {
    char exp;
  };

  std::istream & operator>>(std::istream & input, Point & dest);
  std::istream & operator>>(std::istream & input, Polygon & dest);
  std::istream & operator>>(std::istream & input, DelimiterIO && dest);
}

#endif
