#include <sstream>
#include <algorithm>
#include <limits>
#include "commands.hpp"
#include "vector.hpp"
#include "hashIters.hpp"

void ivantsova::printInvalid(std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
}

void ivantsova::cmdGraphs(std::istream&, std::ostream& out, ivantsova::GraphSet& graphs)
{
  ivantsova::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.pushBack((*it).first);
  }
  if (names.getSize() > 1) {
    for (size_t i = 0; i < names.getSize() - 1; ++i) {
      for (size_t j = 0; j < names.getSize() - i - 1; ++j) {
        if (names[j] > names[j + 1]) {
          std::swap(names[j], names[j + 1]);
        }
      }
    }
  }
  for (size_t i = 0; i < names.getSize(); ++i) {
    out << names[i] << '\n';
  }
  if (names.getSize() == 0) {
    out << '\n';
  }
}

void ivantsova::cmdVertexes(std::istream& in, std::ostream& out, ivantsova::GraphSet& graphs)
{
  std::string name;
  in >> name;
  if (!graphs.has(name)) {
    throw std::runtime_error("Graph not found");
  }
  ivantsova::Vector< std::string > verts = graphs.get(name).getVertices();
  if (verts.getSize() > 1) {
    for (size_t i = 0; i < verts.getSize() - 1; ++i) {
      for (size_t j = 0; j < verts.getSize() - i - 1; ++j) {
        if (verts[j] > verts[j + 1]) {
          std::swap(verts[j], verts[j + 1]);
        }
      }
    }
  }
  for (size_t i = 0; i < verts.getSize(); ++i) {
    out << verts[i] << '\n';
  }
  if (verts.getSize() == 0) {
    out << '\n';
  }
}

void ivantsova::cmdOutbound(std::istream& in, std::ostream& out, ivantsova::GraphSet& graphs)
{
  std::string gname, v;
  in >> gname >> v;
  if (!graphs.has(gname)) {
    throw std::runtime_error("Graph not found");
  }
  if (!graphs.get(gname).hasVertex(v)) {
    throw std::runtime_error("Vertex not found");
  }
  auto res = graphs.get(gname).getOutbound(v);
  if (res.getSize() == 0) {
    out << '\n';
  } else {
    for (size_t i = 0; i < res.getSize(); ++i) {
      out << res[i].first;
      for (size_t w = 0; w < res[i].second.getSize(); ++w) {
        out << " " << res[i].second[w];
      }
      out << '\n';
    }
  }
}

void ivantsova::cmdInbound(std::istream& in, std::ostream& out, ivantsova::GraphSet& graphs)
{
  std::string gname, v;
  in >> gname >> v;
  if (!graphs.has(gname)) {
    throw std::runtime_error("Graph not found");
  }
  if (!graphs.get(gname).hasVertex(v)) {
    throw std::runtime_error("Vertex not found");
  }
  auto res = graphs.get(gname).getInbound(v);
  if (res.getSize() == 0) {
    out << '\n';
  } else {
    for (size_t i = 0; i < res.getSize(); ++i) {
      out << res[i].first;
      for (size_t w = 0; w < res[i].second.getSize(); ++w) {
        out << " " << res[i].second[w];
      }
      out << '\n';
    }
  }
}

void ivantsova::cmdBind(std::istream& in, std::ostream&, ivantsova::GraphSet& graphs)
{
  std::string gname, a, b;
  unsigned long long w;
  in >> gname >> a >> b >> w;
  if (!graphs.has(gname)) {
    throw std::runtime_error("Graph not found");
  }
  graphs.get(gname).addEdge(a, b, w);
}

void ivantsova::cmdCut(std::istream& in, std::ostream&, ivantsova::GraphSet& graphs)
{
  std::string gname, a, b;
  unsigned long long w;
  in >> gname >> a >> b >> w;
  if (!graphs.has(gname)) {
    throw std::runtime_error("Graph not found");
  }
  if (!graphs.get(gname).removeEdge(a, b, w)) {
    throw std::runtime_error("Edge not found");
  }
}

void ivantsova::cmdCreate(std::istream& in, std::ostream&, ivantsova::GraphSet& graphs)
{
  std::string name;
  in >> name;
  if (graphs.has(name)) {
    throw std::runtime_error("Graph already exists");
  }
  size_t k;
  if (!(in >> k)) {
    throw std::runtime_error("Invalid command");
  }
  ivantsova::Graph g;
  for (size_t i = 0; i < k; ++i) {
    std::string v;
    if (!(in >> v)) {
      throw std::runtime_error("Invalid command");
    }
    g.addVertex(v);
  }
  graphs.add(name, g);
}

void ivantsova::cmdMerge(std::istream& in, std::ostream&, ivantsova::GraphSet& graphs)
{
  std::string newname, old1, old2;
  in >> newname >> old1 >> old2;
  if (graphs.has(newname)) {
    throw std::runtime_error("New graph already exists");
  }
  if (!graphs.has(old1)) {
    throw std::runtime_error("First source graph not found");
  }
  if (!graphs.has(old2)) {
    throw std::runtime_error("Second source graph not found");
  }
  graphs.add(newname, graphs.get(old1).merge(graphs.get(old2)));
}

void ivantsova::cmdExtract(std::istream& in, std::ostream&, ivantsova::GraphSet& graphs)
{
  std::string newname, oldname;
  in >> newname >> oldname;
  if (graphs.has(newname)) {
    throw std::runtime_error("New graph already exists");
  }
  if (!graphs.has(oldname)) {
    throw std::runtime_error("Graph not found");
  }
  size_t k;
  if (!(in >> k)) {
  throw std::runtime_error("Invalid command");
  }
  ivantsova::Vector< std::string > verts;
  for (size_t i = 0; i < k; ++i) {
    std::string v;
    in >> v;
    verts.pushBack(v);
  }
  const ivantsova::Graph& old = graphs.get(oldname);
  for (size_t i = 0; i < verts.getSize(); ++i) {
    if (!old.hasVertex(verts[i])) {
      throw std::runtime_error("Vertex not found");
    }
  }
  graphs.add(newname, old.extract(verts));
}
