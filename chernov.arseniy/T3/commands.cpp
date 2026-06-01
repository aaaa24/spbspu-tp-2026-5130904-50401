#include "commands.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <numeric>
#include <string>

void chernov::runCommands(std::istream & input, std::ostream & output,
  const std::unordered_map< std::string, cmd_t > & cmds, std::vector< Polygon > & polygons)
{
  std::string str = "";
  input >> str;

  if (input) {
    if (cmds.count(str)) {
      cmds.at(str)(input, output, polygons);
    }
  }

  if (input.eof()) {
    return;
  }

  if (!input) {
    input.clear();
    input.ignore(std::numeric_limits< std::streamsize >::max());
  }

  runCommands(input, output, cmds, polygons);
}

void chernov::cmdArea(std::istream & input, std::ostream & output, std::vector< Polygon > & polygons)
{
  std::string param = "";
  if (!(input >> param)) {
    return;
  }

  size_t num_of_vertex = 0;
  try {
    size_t num_of_vertex = std::stoull(param);
  } catch (...) {
    if (param == "EVEN" || param == "ODD") {

    } else if (param == "MEAN") {

    } else {
      output << "<INVALID COMMAND>\n";
      return;
    }
  }
}

double chernov::detail::calcSquare(const Polygon & polygon)
{
  std::vector< int > terms;
  terms.reserve(2 * polygon.points.size());

  using namespace std::placeholders;
  auto get_x = std::mem_fn(&Point::x);
  auto get_y = std::mem_fn(&Point::y);
  auto op_get_x = std::bind(get_x, _1);
  auto op_get_y = std::bind(get_y, _2);
  auto operation1 = std::bind(std::multiplies<>{}, op_get_x, op_get_y);
  auto operation2 = std::bind(std::multiplies<>{}, -1, std::bind(std::multiplies<>{}, op_get_x, op_get_y));

  auto begin = polygon.points.begin();
  auto end = polygon.points.end();
  std::transform(begin, std::prev(end), std::next(begin), std::back_inserter(terms), operation1);
  std::transform(std::next(begin), end, begin, std::back_inserter(terms), operation2);

  terms.push_back(polygon.points.back().x * polygon.points.front().y);
  terms.push_back(-1 * polygon.points.front().x * polygon.points.back().y);

  double square = 0.5 * std::abs(std::accumulate(terms.begin(), terms.end(), 0.0));
  return square;
}
