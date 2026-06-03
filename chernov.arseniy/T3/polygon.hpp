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

  std::istream & operator>>(std::istream & input, Point & dest);
  std::istream & operator>>(std::istream & input, Polygon & dest);

  void inputPolygons(std::istream & input, std::vector< Polygon > & polygons);

  namespace detail {
    struct DelimiterIO {
      char exp;
    };

    std::istream & operator>>(std::istream & input, DelimiterIO && dest);

    void skipSpaces(std::istream & input);
    bool isLineEnd(std::istream & input);
  }
}

#endif
