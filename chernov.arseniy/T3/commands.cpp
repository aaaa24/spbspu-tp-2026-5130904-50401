#include "commands.hpp"

#include <limits>
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
