#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "bstree.hpp"

namespace ivantsova {
  using Dictionary = BSTree< int, std::string >;
  using Datasets = BSTree< std::string, Dictionary >;
  using Command = void (*)(std::istream &, std::ostream &, Datasets &);

  void cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdComplement(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdIntersect(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdUnion(std::istream &in, std::ostream &out, Datasets &datasets);
}

#endif
