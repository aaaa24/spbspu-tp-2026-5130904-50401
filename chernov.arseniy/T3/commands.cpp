#include "commands.hpp"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <string>

void chernov::runCommands(std::istream & input, std::ostream & output,
  const std::unordered_map< std::string, cmd_t > & cmds, const std::vector< Polygon > & polygons)
{
  std::string str = "";
  input >> str;

  if (input) {
    if (cmds.count(str)) {
      cmds.at(str)(input, output, polygons);
    } else {
      output << "<INVALID COMMAND>\n";
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

void chernov::cmdArea(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  std::string param = "";
  if (!(input >> param)) {
    return;
  }

  double area = 0.0;
  try {
    size_t num_of_vertexes = std::stoull(param);
    area = detail::calcAreaSumWithNumOfVertexes(polygons, num_of_vertexes);
  } catch (...) {
    if (param == "EVEN" || param == "ODD") {
      area = detail::calcAreaSum(polygons, param);
    } else if (param == "MEAN") {
      area = detail::calcAreaMean(polygons);
    } else {
      output << "<INVALID COMMAND>\n";
      return;
    }
  }

  output << std::fixed << std::setprecision(1) << area << "\n";
}

template< class Container >
std::vector< double > chernov::detail::getAreas(const Container & polygons)
{
  std::vector< double > areas;
  areas.reserve(polygons.size());
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), calcArea);
  return areas;
}

template< class Container >
double chernov::detail::getSumOfAreas(const Container & polygons)
{
  std::vector< double > areas = getAreas(polygons);
  double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
  return sum;
}

double chernov::detail::calcArea(const Polygon & polygon)
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

  double area = 0.5 * std::abs(std::accumulate(terms.begin(), terms.end(), 0.0));
  return area;
}

double chernov::detail::calcAreaMean(const std::vector< Polygon > & polygons)
{
  if (polygons.size() == 0) {
    return 0.0;
  }

  double area = getSumOfAreas(polygons) / polygons.size();
  return area;
}

double chernov::detail::calcAreaSum(const std::vector< Polygon > & polygons, const std::string & param)
{
  std::vector< std::reference_wrapper< const Polygon > > filtered;

  using namespace std::placeholders;
  auto get_points = std::mem_fn(&Polygon::points);
  auto get_size = std::mem_fn(&std::vector< Point >::size);
  auto poly_size = std::bind(get_size, std::bind(get_points, _1));
  auto odd = std::bind(std::modulus< size_t >{}, poly_size, 2);
  auto even = std::bind(std::logical_not< bool >{}, odd);

  if (param == "EVEN") {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), even);
  } else if (param == "ODD") {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), odd);
  } else {
    throw std::invalid_argument("param must be \"even\" or \"odd\"");
  }

  return getSumOfAreas(filtered);
}

double chernov::detail::calcAreaSumWithNumOfVertexes(const std::vector< Polygon > & polygons, size_t num_of_vertexes)
{
  std::vector< std::reference_wrapper< const Polygon > > filtered;

  using namespace std::placeholders;
  auto get_points = std::mem_fn(&Polygon::points);
  auto get_size = std::mem_fn(&std::vector< Point >::size);
  auto poly_size = std::bind(get_size, std::bind(get_points, _1));
  auto pred = std::bind(std::equal_to< size_t >{}, poly_size, num_of_vertexes);

  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);

  return getSumOfAreas(filtered);
}
