#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include "hashTable.hpp"
#include "vector.hpp"

namespace ivantsova {

  class Graph
  {
  public:
    Graph();

    void addVertex(const std::string& name);
    void addEdge(const std::string& from, const std::string& to, unsigned long long weight);
    bool removeEdge(const std::string& from, const std::string& to, unsigned long long weight);
    bool hasVertex(const std::string& name) const;

    Vector< std::string > getVertices() const;

    Vector< std::pair< std::string, Vector< unsigned long long > > >
    getOutbound(const std::string& vertex) const;
    Vector< std::pair< std::string, Vector< unsigned long long > > >
    getInbound(const std::string& vertex) const;

    Graph merge(const Graph& other) const;
    Graph extract(const Vector< std::string >& vertices) const;

  private:
    using EdgeKey = std::pair< std::string, std::string >;
    using EdgeTable = HashTable<EdgeKey, Vector< unsigned long long >, PairHash< std::string >, PairEqual< std::string > >;

    Vector< std::string > vertices_;
    EdgeTable edges_;
  };
}

#endif
