#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "hashTable.hpp"
#include "graph.hpp"

namespace ivantsova {

  using GraphSet = HashTable< std::string, Graph >;

  void cmdGraphs(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdVertexes(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdOutbound(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdInbound(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdBind(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdCut(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdCreate(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdMerge(std::istream& in, std::ostream& out, GraphSet& graphs);
  void cmdExtract(std::istream& in, std::ostream& out, GraphSet& graphs);
  void printInvalid(std::ostream& out);
}

#endif
