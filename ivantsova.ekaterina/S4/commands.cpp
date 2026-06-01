#include <stdexcept>
#include "commands.hpp"

void ivantsova::cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets)
{
  std::string name;
  if (!(in >> name)) {
    throw std::runtime_error("Invalid input");
  }
  Datasets::iterator it = datasets.find(name);
  if (it == datasets.end()) {
    throw std::runtime_error("Dataset not found");
  }
  const Dictionary &dict = (*it).second;
  if (dict.empty()) {
    out << "<EMPTY>" << "\n";
  } else {
    out << name;
    for (Dictionary::const_iterator cit = dict.cbegin(); cit != dict.cend(); ++cit) {
      out << " " << (*cit).first << " " << (*cit).second;
    }
    out << "\n";
  }
}

void ivantsova::cmdComplement(std::istream &in, std::ostream &, Datasets &datasets)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  Datasets::iterator it1 = datasets.find(name1);
  Datasets::iterator it2 = datasets.find(name2);
  if (it1 == datasets.end() || it2 == datasets.end()) {
    throw std::runtime_error("Dataset not found");
  }
  const Dictionary &ds1 = (*it1).second;
  const Dictionary &ds2 = (*it2).second;
  Dictionary newDs;
  for (Dictionary::const_iterator cit = ds1.cbegin(); cit != ds1.cend(); ++cit) {
    if (ds2.find((*cit).first) == ds2.end()) {
      newDs.push((*cit).first, (*cit).second);
    }
  }
  datasets.push(newName, newDs);
}

void ivantsova::cmdIntersect(std::istream &in, std::ostream &, Datasets &datasets)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  Datasets::iterator it1 = datasets.find(name1);
  Datasets::iterator it2 = datasets.find(name2);
  if (it1 == datasets.end() || it2 == datasets.end()) {
    throw std::runtime_error("Dataset not found");
  }
  const Dictionary &ds1 = (*it1).second;
  const Dictionary &ds2 = (*it2).second;
  Dictionary newDs;
  for (Dictionary::const_iterator cit = ds1.cbegin(); cit != ds1.cend(); ++cit) {
    if (ds2.find((*cit).first) != ds2.end()) {
      newDs.push((*cit).first, (*cit).second);
    }
  }
  datasets.push(newName, newDs);
}

void ivantsova::cmdUnion(std::istream &in, std::ostream &, Datasets &datasets)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  Datasets::iterator it1 = datasets.find(name1);
  Datasets::iterator it2 = datasets.find(name2);
  if (it1 == datasets.end() || it2 == datasets.end()) {
    throw std::runtime_error("Dataset not found");
  }
  const Dictionary &ds1 = (*it1).second;
  const Dictionary &ds2 = (*it2).second;
  Dictionary newDs;
  for (Dictionary::const_iterator cit = ds1.cbegin(); cit != ds1.cend(); ++cit) {
    newDs.push((*cit).first, (*cit).second);
  }
  for (Dictionary::const_iterator cit = ds2.cbegin(); cit != ds2.cend(); ++cit) {
    if (newDs.find((*cit).first) == newDs.end()) {
      newDs.push((*cit).first, (*cit).second);
    }
  }
  datasets.push(newName, newDs);
}
