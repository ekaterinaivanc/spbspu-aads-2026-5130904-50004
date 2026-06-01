#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include "hashTable.hpp"
#include "hashIters.hpp"
#include "graph.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Invalid number of arguments" << "\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file" << "\n";
    return 1;
  }

  ivantsova::GraphSet graphs;
  std::string graphName;
  size_t edgeCount;

  while (file >> graphName >> edgeCount) {
    ivantsova::Graph g;
    for (size_t i = 0; i < edgeCount; ++i) {
      std::string from, to;
      unsigned long long weight;
      file >> from >> to >> weight;
      g.addEdge(from, to, weight);
    }
    graphs.add(graphName, std::move(g));
  }

  ivantsova::HashTable< std::string, void(*)(std::istream&, std::ostream&, ivantsova::GraphSet&) > commands(17);
  commands.add("graphs", ivantsova::cmdGraphs);
  commands.add("vertexes", ivantsova::cmdVertexes);
  commands.add("outbound", ivantsova::cmdOutbound);
  commands.add("inbound", ivantsova::cmdInbound);
  commands.add("bind", ivantsova::cmdBind);
  commands.add("cut", ivantsova::cmdCut);
  commands.add("create", ivantsova::cmdCreate);
  commands.add("merge", ivantsova::cmdMerge);
  commands.add("extract", ivantsova::cmdExtract);

  std::string cmd;
  while (std::cin >> cmd) {
    if (!commands.has(cmd)) {
      ivantsova::printInvalid(std::cout);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    try {
      commands.get(cmd)(std::cin, std::cout, graphs);
    } catch (const std::exception&) {
      ivantsova::printInvalid(std::cout);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
