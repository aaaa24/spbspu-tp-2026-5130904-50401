#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>
#include <unordered_map>

#include "polygon.hpp"

namespace chernov {
  using cmd_t = void(*)(std::istream &, std::ostream &, std::vector< Polygon > &);

  void runCommands(std::istream & input, std::ostream & output,
    const std::unordered_map< std::string, cmd_t > & cmds, std::vector< Polygon > & polygons);

  void cmdArea(std::istream &, std::ostream &, std::vector< Polygon > &);

  namespace detail {
    double calcArea(const Polygon & polygon);
    double calcAreaMean(const std::vector< Polygon > & polygons);
  }
}

#endif
