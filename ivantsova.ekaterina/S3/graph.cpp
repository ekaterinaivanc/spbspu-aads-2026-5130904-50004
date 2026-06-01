#include <algorithm>
#include <stdexcept>
#include "hashIters.hpp"
#include "graph.hpp"

ivantsova::Graph::Graph():
 vertices_(),
 edges_()
{}

void ivantsova::Graph::addVertex(const std::string& name)
{
  for (size_t i = 0; i < vertices_.getSize(); ++i) {
    if (vertices_[i] == name) {
      return;
    }
  }
  vertices_.pushBack(name);
}

bool ivantsova::Graph::hasVertex(const std::string& name) const
{
  for (size_t i = 0; i < vertices_.getSize(); ++i) {
    if (vertices_[i] == name) {
      return true;
    }
  }
  return false;
}

ivantsova::Vector< std::string > ivantsova::Graph::getVertices() const
{
  return vertices_;
}

void ivantsova::Graph::addEdge(const std::string& from, const std::string& to, unsigned long long weight)
{
  addVertex(from);
  addVertex(to);
  EdgeKey key(from, to);
  if (edges_.has(key)) {
    edges_.get(key).pushBack(weight);
  } else {
    Vector<unsigned long long> weights;
    weights.pushBack(weight);
    edges_.add(key, weights);
  }
}

bool ivantsova::Graph::removeEdge(const std::string& from, const std::string& to, unsigned long long weight)
{
  if (!hasVertex(from) || !hasVertex(to)) {
    return false;
  }
  EdgeKey key(from, to);
  if (!edges_.has(key)) {
    return false;
  }
  Vector< unsigned long long >& weights = edges_.get(key);
  for (size_t i = 0; i < weights.getSize(); ++i) {
    if (weights[i] == weight) {
      for (size_t j = i; j < weights.getSize() - 1; ++j) {
        weights[j] = weights[j + 1];
      }
      weights.popBack();
      if (weights.getSize() == 0) {
        edges_.drop(key);
      }
      return true;
    }
  }
  return false;
}

ivantsova::Vector< std::pair< std::string, ivantsova::Vector< unsigned long long > > >
ivantsova::Graph::getOutbound(const std::string& vertex) const
{
  Vector< std::pair< std::string, Vector< unsigned long long > > > result;
  if (vertices_.getSize() == 0) {
    return result;
  }
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    const EdgeKey& key = (*it).first;
    if (key.first == vertex) {
      const Vector< unsigned long long >& weights = (*it).second;
      bool found = false;
      for (size_t i = 0; i < result.getSize(); ++i) {
        if (result[i].first == key.second) {
          found = true;
          for (size_t w = 0; w < weights.getSize(); ++w) {
            result[i].second.pushBack(weights[w]);
          }
          break;
        }
      }
      if (!found) {
        result.pushBack({key.second, weights});
      }
    }
  }
  for (size_t i = 0; i < result.getSize(); ++i) {
    Vector< unsigned long long >& w = result[i].second;
    if (w.getSize() > 1) {
      for (size_t j = 0; j < w.getSize() - 1; ++j) {
        for (size_t k = 0; k < w.getSize() - j - 1; ++k) {
          if (w[k] > w[k + 1]) {
            std::swap(w[k], w[k + 1]);
          }
        }
      }
    }
  }
  if (result.getSize() > 1) {
    for (size_t i = 0; i < result.getSize() - 1; ++i) {
      for (size_t j = 0; j < result.getSize() - i - 1; ++j) {
        if (result[j].first > result[j + 1].first) {
          std::swap(result[j], result[j + 1]);
        }
      }
    }
  }
  return result;
}

ivantsova::Vector< std::pair< std::string, ivantsova::Vector< unsigned long long > > >
ivantsova::Graph::getInbound(const std::string& vertex) const {
  Vector< std::pair< std::string, Vector< unsigned long long > > > result;
  if (vertices_.getSize() == 0) {
    return result;
  }
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    const EdgeKey& key = (*it).first;
    if (key.second == vertex) {
      const Vector< unsigned long long >& weights = (*it).second;
      bool found = false;
      for (size_t i = 0; i < result.getSize(); ++i) {
        if (result[i].first == key.first) {
          found = true;
          for (size_t w = 0; w < weights.getSize(); ++w) {
            result[i].second.pushBack(weights[w]);
          }
          break;
        }
      }
      if (!found) {
        result.pushBack({key.first, weights});
      }
    }
  }
  for (size_t i = 0; i < result.getSize(); ++i) {
    Vector< unsigned long long >& w = result[i].second;
    if (w.getSize() > 1) {
      for (size_t j = 0; j < w.getSize() - 1; ++j) {
        for (size_t k = 0; k < w.getSize() - j - 1; ++k) {
          if (w[k] > w[k + 1]) {
            std::swap(w[k], w[k + 1]);
          }
        }
      }
    }
  }
  if (result.getSize() > 1) {
    for (size_t i = 0; i < result.getSize() - 1; ++i) {
      for (size_t j = 0; j < result.getSize() - i - 1; ++j) {
        if (result[j].first > result[j + 1].first) {
          std::swap(result[j], result[j + 1]);
        }
      }
    }
  }
  return result;
}

ivantsova::Graph ivantsova::Graph::merge(const Graph& other) const
{
  Graph result;
  for (size_t i = 0; i < vertices_.getSize(); ++i) {
    result.addVertex(vertices_[i]);
  }
  Vector< std::string > otherVerts = other.getVertices();
  for (size_t i = 0; i < otherVerts.getSize(); ++i) {
    result.addVertex(otherVerts[i]);
  }
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    const EdgeKey& key = (*it).first;
    const Vector< unsigned long long >& weights = (*it).second;
    for (size_t w = 0; w < weights.getSize(); ++w) {
      result.addEdge(key.first, key.second, weights[w]);
    }
  }
  for (auto it = other.edges_.begin(); it != other.edges_.end(); ++it) {
    const EdgeKey& key = (*it).first;
    const Vector< unsigned long long >& weights = (*it).second;
    for (size_t w = 0; w < weights.getSize(); ++w) {
      result.addEdge(key.first, key.second, weights[w]);
    }
  }
  return result;
}

ivantsova::Graph ivantsova::Graph::extract(const Vector< std::string >& vertices) const
{
  Graph result;
  for (size_t i = 0; i < vertices.getSize(); ++i) {
    if (hasVertex(vertices[i])) {
      result.addVertex(vertices[i]);
    }
  }
  auto isSelected = [&vertices](const std::string& v) -> bool {
    for (size_t i = 0; i < vertices.getSize(); ++i) {
      if (vertices[i] == v) {
        return true;
      }
    }
    return false;
  };
  for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    const EdgeKey& key = (*it).first;
    if (isSelected(key.first) && isSelected(key.second)) {
      const Vector< unsigned long long >& weights = (*it).second;
      for (size_t w = 0; w < weights.getSize(); ++w) {
        result.addEdge(key.first, key.second, weights[w]);
      }
    }
  }
  return result;
}
