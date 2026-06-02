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
    try {
      cmds.at(str)(input, output, polygons);
    } catch(...) {
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      output << "<INVALID COMMAND>\n";
    }
  }

  if (input.eof()) {
    return;
  }

  if (!input) {
    input.clear();
    input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
        throw std::runtime_error("invalid command\n");
    }
  }

  output << std::fixed << std::setprecision(1) << area << "\n";
}

void chernov::cmdMax(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  std::string param = "";
  if (!(input >> param)) {
    return;
  }

  if (param == "AREA") {
    double max_area = detail::getMinMaxArea(polygons).second;
    output << std::fixed << std::setprecision(1) << max_area << "\n";
  } else if (param == "VERTEXES") {
    size_t max_count_vertexes = detail::getMinMaxCountVertexes(polygons).second;
    output << max_count_vertexes << "\n";
  } else {
    throw std::runtime_error("invalid command\n");
  }
}

void chernov::cmdMin(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  std::string param = "";
  if (!(input >> param)) {
    return;
  }

  if (param == "AREA") {
    double min_area = detail::getMinMaxArea(polygons).first;
    output << std::fixed << std::setprecision(1) << min_area << "\n";
  } else if (param == "VERTEXES") {
    size_t min_count_vertexes = detail::getMinMaxCountVertexes(polygons).first;
    output << min_count_vertexes << "\n";
  } else {
    throw std::runtime_error("invalid command\n");
  }
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
    throw std::invalid_argument("poligon's size must be greater than 0");
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
  auto get_count_vertexes = std::bind(get_size, std::bind(get_points, _1));
  auto odd = std::bind(std::modulus< size_t >{}, get_count_vertexes, 2);
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
  auto get_count_vertexes = std::bind(get_size, std::bind(get_points, _1));
  auto pred = std::bind(std::equal_to< size_t >{}, get_count_vertexes, num_of_vertexes);

  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);

  return getSumOfAreas(filtered);
}

std::pair< double, double > chernov::detail::getMinMaxArea(const std::vector< Polygon > & polygons)
{
  if (polygons.size() == 0) {
    throw std::invalid_argument("poligon's size must be greater than 0");
  }

  std::vector< double > areas = getAreas(polygons);
  auto minmax = std::minmax_element(areas.begin(), areas.end());
  return {*minmax.first, *minmax.second};
}

std::pair< size_t, size_t > chernov::detail::getMinMaxCountVertexes(const std::vector< Polygon > & polygons)
{
  if (polygons.size() == 0) {
    throw std::invalid_argument("poligon's size must be greater than 0");
  }

  using namespace std::placeholders;
  auto get_points = std::mem_fn(&Polygon::points);
  auto get_size = std::mem_fn(&std::vector< Point >::size);
  auto get_count_vertexes = std::bind(get_size, std::bind(get_points, _1));

  std::vector< size_t > vertexes;
  vertexes.reserve(polygons.size());
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(vertexes), get_count_vertexes);

  auto minmax = std::minmax_element(vertexes.begin(), vertexes.end());
  return {*minmax.first, *minmax.second};
}
