#include "input.hpp"

#include <limits>

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
