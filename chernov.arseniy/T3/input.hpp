#ifndef INPUT_HPP
#define INPUT_HPP

#include <iosfwd>
#include <vector>

#include "polygon.hpp"

namespace chernov {
  void inputPolygons(std::istream & input, std::vector< Polygon > & polygons);
}

#endif
