#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iosfwd>
#include <vector>
#include <unordered_map>

#include "polygon.hpp"

namespace chernov {
  using cmd_t = std::function< void(std::istream &, std::ostream &, const std::vector< Polygon > &) >;

  struct CommandIO {
    std::ostream & output;
    const std::unordered_map< std::string, cmd_t > & cmds;
    const std::vector< Polygon > & polygons;
  };

  struct EndlIO {};

  std::istream & operator>>(std::istream & input, CommandIO & cmd_io);
  std::istream & operator>>(std::istream & input, EndlIO &&);

  void runCommands(std::istream & input, CommandIO & cmd_io);

  void cmdArea(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void cmdMax(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void cmdMin(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void cmdCount(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void cmdInframe(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void cmdSame(std::istream &, std::ostream &, const std::vector< Polygon > &);

  namespace detail {
    template< class Container >
    std::vector< double > getAreas(const Container & polygons);

    template< class Container >
    double getSumOfAreas(const Container & polygons);

    double calcArea(const Polygon & polygon);
    double calcAreaMean(const std::vector< Polygon > & polygons);
    double calcAreaSum(const std::vector< Polygon > & polygons, const std::string & param);
    double calcAreaSumWithNumOfVertexes(const std::vector< Polygon > & polygons, size_t num_of_vertexes);
    std::pair< double, double > getMinMaxArea(const std::vector< Polygon > & polygons);
    std::vector< size_t > getVertexes(const std::vector< Polygon > & polygons);
    std::pair< size_t, size_t > getMinMaxCountVertexes(const std::vector< Polygon > & polygons);
    size_t countPolygonsWithNumOfVertexes(const std::vector< Polygon > & polygons, size_t num_of_vertexes);
    size_t countPolygonsWithParam(const std::vector< Polygon > & polygons, const std::string & param);
    std::pair< Point, Point > getBoundingFrame(const Polygon & polygon);
    std::pair< Point, Point > getBoundingFrame(const std::vector< Polygon > & polygons);
    bool isSameWithShift(const Polygon & p1, const Polygon & p2, size_t shift);
    bool isSame(const Polygon & p1, const Polygon & p2);
  }
}

#endif
