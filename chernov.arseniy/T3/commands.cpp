#include "commands.hpp"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <string>

std::istream & chernov::operator>>(std::istream & input, CommandIO & cmd_io)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  std::string str = "";
  input >> str;

  if (input) {
    try {
      cmd_io.cmds.at(str)(input, cmd_io.output, cmd_io.polygons);
    } catch (...) {
      input.setstate(std::ios::failbit);
    }
  }

  if (!input) {
    input.clear();
    input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    cmd_io.output << "<INVALID COMMAND>\n";
  }

  return input;
}

std::istream & chernov::operator>>(std::istream & input, EndlIO &&)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }

  char c = 0;
  input >> c;
  if (input && c != '\n') {
    input.setstate(std::ios::failbit);
  }

  return input;
}

void chernov::runCommands(std::istream & input, CommandIO & cmd_io)
{
  if (!(input >> cmd_io)) {
    return;
  }
  runCommands(input, cmd_io);
}

void chernov::cmdArea(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  std::string param = "";
  if (!(input >> param)) {
    throw std::runtime_error("invalid command\n");
  }

  double area = 0.0;
  try {
    size_t num_of_vertexes = std::stoull(param);
    if (num_of_vertexes < 3) {
      throw std::runtime_error("invalid command\n");
    }
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
    throw std::runtime_error("invalid command\n");
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
    throw std::runtime_error("invalid command\n");
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

void chernov::cmdCount(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  std::string param = "";
  if (!(input >> param)) {
    throw std::runtime_error("invalid command\n");
  }

  size_t count_polygons = 0;
  try {
    size_t num_of_vertexes = std::stoull(param);
    if (num_of_vertexes < 3) {
      throw std::runtime_error("invalid command\n");
    }
    count_polygons = detail::countPolygonsWithNumOfVertexes(polygons, num_of_vertexes);
  } catch (...) {
    if (param == "EVEN" || param == "ODD") {
      count_polygons = detail::countPolygonsWithParam(polygons, param);
    } else {
        throw std::runtime_error("invalid command\n");
    }
  }

  output << count_polygons << "\n";
}

void chernov::cmdInframe(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  Polygon polygon;
  if (!(input >> polygon)) {
    throw std::runtime_error("invalid command\n");
  }

  std::pair< Point, Point > frame = detail::getBoundingFrame(polygons);

  using namespace std::placeholders;
  auto get_x = std::mem_fn(&Point::x);
  auto get_y = std::mem_fn(&Point::y);
  auto comp_x = std::bind(std::logical_and<>{},
    std::bind(std::greater_equal<>{}, std::bind(get_x, _1), frame.first.x),
    std::bind(std::less_equal<>{}, std::bind(get_x, _1), frame.second.x)
  );
  auto comp_y = std::bind(std::logical_and<>{},
    std::bind(std::greater_equal<>{}, std::bind(get_y, _1), frame.first.y),
    std::bind(std::less_equal<>{}, std::bind(get_y, _1), frame.second.y)
  );
  auto pred = std::bind(std::logical_and<>{}, comp_x, comp_y);

  size_t count = std::count_if(polygon.points.begin(), polygon.points.end(), pred);
  if (count == polygon.points.size()) {
    output << "<TRUE>\n";
  } else {
    output << "<FALSE>\n";
  }
}

void chernov::cmdSame(std::istream & input, std::ostream & output, const std::vector< Polygon > & polygons)
{
  Polygon polygon;
  if (!(input >> polygon)) {
    throw std::runtime_error("invalid command\n");
  }

  using namespace std::placeholders;
  auto pred = std::bind(detail::isSame, _1, std::ref(polygon));
  size_t count = std::count_if(polygons.begin(), polygons.end(), pred);
  output << count << "\n";
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
  auto odd = std::bind(std::modulus<>{}, get_count_vertexes, 2);
  auto even = std::bind(std::logical_not<>{}, odd);

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

std::vector< size_t > chernov::detail::getVertexes(const std::vector< Polygon > & polygons)
{
  using namespace std::placeholders;
  auto get_points = std::mem_fn(&Polygon::points);
  auto get_size = std::mem_fn(&std::vector< Point >::size);
  auto get_count_vertexes = std::bind(get_size, std::bind(get_points, _1));

  std::vector< size_t > vertexes;
  vertexes.reserve(polygons.size());
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(vertexes), get_count_vertexes);

  return vertexes;
}

std::pair< size_t, size_t > chernov::detail::getMinMaxCountVertexes(const std::vector< Polygon > & polygons)
{
  if (polygons.size() == 0) {
    throw std::invalid_argument("poligon's size must be greater than 0");
  }

  std::vector< size_t > vertexes = getVertexes(polygons);

  auto minmax = std::minmax_element(vertexes.begin(), vertexes.end());
  return {*minmax.first, *minmax.second};
}

size_t chernov::detail::countPolygonsWithNumOfVertexes(const std::vector< Polygon > & polygons, size_t num_of_vertexes)
{
  std::vector< size_t > vertexes = getVertexes(polygons);

  using namespace std::placeholders;
  auto pred = std::bind(std::equal_to<>{}, _1, num_of_vertexes);
  return std::count_if(vertexes.begin(), vertexes.end(), pred);
}

size_t chernov::detail::countPolygonsWithParam(const std::vector< Polygon > & polygons, const std::string & param)
{
  std::vector< size_t > vertexes = getVertexes(polygons);

  using namespace std::placeholders;
  auto odd = std::bind(std::modulus<>{}, _1, 2);
  auto even = std::bind(std::logical_not<>{}, odd);

  if (param == "EVEN") {
    return std::count_if(vertexes.begin(), vertexes.end(), even);
  } else if (param == "ODD") {
    return std::count_if(vertexes.begin(), vertexes.end(), odd);
  } else {
    throw std::invalid_argument("param must be \"even\" or \"odd\"");
  }
}

std::pair< chernov::Point, chernov::Point > chernov::detail::getBoundingFrame(const Polygon & polygon)
{
  using namespace std::placeholders;
  auto get_x = std::mem_fn(&Point::x);
  auto get_y = std::mem_fn(&Point::y);

  auto less_x = std::bind(std::less<>(), std::bind(get_x, _1), std::bind(get_x, _2));
  auto less_y = std::bind(std::less<>(), std::bind(get_y, _1), std::bind(get_y, _2));

  auto it_min_x = std::min_element(polygon.points.begin(), polygon.points.end(), less_x);
  auto it_max_x = std::max_element(polygon.points.begin(), polygon.points.end(), less_x);
  auto it_min_y = std::min_element(polygon.points.begin(), polygon.points.end(), less_y);
  auto it_max_y = std::max_element(polygon.points.begin(), polygon.points.end(), less_y);

  return {Point{it_min_x->x, it_min_y->y}, Point{it_max_x->x, it_max_y->y}};
}

std::pair< chernov::Point, chernov::Point > chernov::detail::getBoundingFrame(const std::vector< Polygon > & polygons)
{
  using namespace std::placeholders;

  std::vector< std::pair< Point, Point > > boxes;
  boxes.reserve(polygons.size());

  std::pair< Point, Point >(*unary_op)(const Polygon &) = getBoundingFrame;
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(boxes), unary_op);

  auto get_x = std::mem_fn(&Point::x);
  auto get_y = std::mem_fn(&Point::y);
  auto get_first = std::mem_fn(&std::pair< Point, Point >::first);
  auto get_second = std::mem_fn(&std::pair< Point, Point >::second);

  auto less_x_first = std::bind(std::less<>(),
      std::bind(get_x, std::bind(get_first, _1)),
      std::bind(get_x, std::bind(get_first, _2)));
  auto less_y_first = std::bind(std::less<>(),
      std::bind(get_y, std::bind(get_first, _1)),
      std::bind(get_y, std::bind(get_first, _2)));

  auto less_x_second = std::bind(std::less<>(),
      std::bind(get_x, std::bind(get_second, _1)),
      std::bind(get_x, std::bind(get_second, _2)));
  auto less_y_second = std::bind(std::less<>(),
      std::bind(get_y, std::bind(get_second, _1)),
      std::bind(get_y, std::bind(get_second, _2)));

  auto it_min_x = std::min_element(boxes.begin(), boxes.end(), less_x_first);
  auto it_min_y = std::min_element(boxes.begin(), boxes.end(), less_y_first);
  auto it_max_x = std::max_element(boxes.begin(), boxes.end(), less_x_second);
  auto it_max_y = std::max_element(boxes.begin(), boxes.end(), less_y_second);

  return {{it_min_x->first.x, it_min_y->first.y}, {it_max_x->second.x, it_max_y->second.y}};
}

bool chernov::detail::isSameWithShift(const Polygon & p1, const Polygon & p2, size_t shift)
{
  size_t n = p1.points.size();
  if (shift >= n) {
      return false;
  }

  std::vector< Point > rotated(n);
  std::rotate_copy(p2.points.begin(), p2.points.begin() + shift, p2.points.end(), rotated.begin());

  int dx = rotated[0].x - p1.points[0].x;
  int dy = rotated[0].y - p1.points[0].y;

  using namespace std::placeholders;
  auto get_x = std::mem_fn(&Point::x);
  auto get_y = std::mem_fn(&Point::y);
  auto calc_x = std::bind(std::minus<>{}, std::bind(get_x, _2), dx);
  auto calc_y = std::bind(std::minus<>{}, std::bind(get_y, _2), dy);
  auto comp_x = std::bind(std::equal_to<>{}, std::bind(get_x, _1), calc_x);
  auto comp_y = std::bind(std::equal_to<>{}, std::bind(get_y, _1), calc_y);
  auto binary_pred = std::bind(std::logical_and<>{}, comp_x, comp_y);

  bool ok = std::equal(p1.points.begin(), p1.points.end(), rotated.begin(), binary_pred);
  if (ok) {
    return true;
  }
  return isSameWithShift(p1, p2, shift + 1);
}

bool chernov::detail::isSame(const Polygon & p1, const Polygon & p2)
{
  if (p1.points.empty() || p2.points.empty()) {
    return p1.points.empty() && p2.points.empty();
  }
  if (p1.points.size() != p2.points.size()) {
    return false;
  }

  if (isSameWithShift(p1, p2, 0)) {
    return true;
  }

  std::vector< Point > reversed;
  reversed.reserve(p1.points.size());
  std::reverse_copy(p2.points.begin(), p2.points.end(), std::back_inserter(reversed));
  Polygon p2_reversed;
  p2_reversed.points = std::move(reversed);
  return isSameWithShift(p1, p2_reversed, 0);
}
