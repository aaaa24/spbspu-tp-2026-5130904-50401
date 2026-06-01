#include "commands.hpp"

#include <limits>

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
