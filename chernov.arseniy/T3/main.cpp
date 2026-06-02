#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include <unordered_map>

#include "commands.hpp"
#include "polygon.hpp"

int main(int argc, char ** argv)
{
  if (argc < 2) {
    std::cerr << "first argument must be name of file\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "could not open file\n";
    return 1;
  }

  std::vector< chernov::Polygon > polygons;
  chernov::inputPolygons(file, polygons);

  std::unordered_map< std::string, chernov::cmd_t > cmds;
  cmds["AREA"] = chernov::cmdArea;
  cmds["MAX"] = chernov::cmdMax;
  cmds["MIN"] = chernov::cmdMin;
  cmds["COUNT"] = chernov::cmdCount;
  cmds["INFRAME"] = chernov::cmdInframe;
  cmds["SAME"] = chernov::cmdSame;

  chernov::runCommands(std::cin, std::cout, cmds, polygons);
}
