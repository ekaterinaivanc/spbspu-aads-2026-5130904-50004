#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include "bstree.hpp"

using namespace ivantsova;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(empty_tree)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.empty());
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  BSTree< int, std::string > original;
  original.push(1, "one");
  original.push(2, "two");

  BSTree< int, std::string > copy(original);

  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK_EQUAL(copy.get(1), "one");
  BOOST_CHECK_EQUAL(copy.get(2), "two");

  original.push(3, "three");
  BOOST_CHECK_EQUAL(copy.size(), 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  BSTree< int, std::string > a;
  a.push(1, "one");
  BSTree< int, std::string > b;
  b.push(2, "two");
  b = a;

  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(b.get(1), "one");
  BOOST_CHECK_THROW(b.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(swap)
{
  BSTree< int, std::string > a;
  a.push(1, "one");
  BSTree< int, std::string > b;
  b.push(2, "two");
  b.push(3, "three");
  a.swap(b);

  BOOST_CHECK_EQUAL(a.size(), 2);
  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(a.get(2), "two");
  BOOST_CHECK_EQUAL(b.get(1), "one");
}

BOOST_AUTO_TEST_CASE(push)
{
  BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(push_updates_key)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(1, "ONE");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(get_returns_value)
{
  BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");

  BOOST_CHECK_EQUAL(tree.get(10), "ten");
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
}

BOOST_AUTO_TEST_CASE(get_missing_key)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
  BOOST_CHECK_THROW(tree.get(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(const_get)
{
  BSTree< int, std::string > tree;
  tree.push(42, "fourtytwo");
  const BSTree< int, std::string >& constTree = tree;

  BOOST_CHECK_EQUAL(constTree.get(42), "fourtytwo");
  BOOST_CHECK_THROW(constTree.get(100), std::out_of_range);
}


BOOST_AUTO_TEST_CASE(drop_removed)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  std::string removed = tree.drop(2);

  BOOST_CHECK_EQUAL(removed, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(drop_missing_key)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.drop(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(find_returns_iterator)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.find(1);
  BOOST_CHECK(it != tree.end());
  BOOST_CHECK_EQUAL((*it).first, 1);
  BOOST_CHECK_EQUAL((*it).second, "one");
}

BOOST_AUTO_TEST_CASE(find_returns_end_on_missing_key)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  auto it = tree.find(42);
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(const_find)
{
  BSTree< int, std::string > tree;
  tree.push(7, "seven");
  const BSTree< int, std::string >& constTree = tree;

  auto it = constTree.find(7);
  BOOST_CHECK(it != constTree.end());
  BOOST_CHECK_EQUAL((*it).first, 7);
}

BOOST_AUTO_TEST_CASE(begin_returns_leftmost_element)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(4, "four");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL((*it).first, 1);
  BOOST_CHECK_EQUAL((*it).second, "one");
}

BOOST_AUTO_TEST_CASE(iterator_traverses)
{
  BSTree< int, std::string > tree;
  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(5, "five");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL((*it).first, 1);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 2);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 3);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 4);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 5);
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(iterator_increment_and_decrement)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL((*it).first, 1);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 2);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 3);
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(const_iterator)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  const BSTree< int, std::string >& constTree = tree;
  auto it = constTree.cbegin();
  BOOST_CHECK_EQUAL((*it).first, 1);
}


BOOST_AUTO_TEST_CASE(height_of_empty_tree)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(height_of_single_node)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(height_of_balanced_tree)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(height_of_linear_tree)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(rotate_left)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  const BSTree< int, std::string >& constTree = tree;
  BSTree< int, std::string >::const_iterator it = constTree.find(1);
  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(rotate_right)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");

  const BSTree< int, std::string >& constTree = tree;
  BSTree< int, std::string >::const_iterator it = constTree.find(2);
  tree.rotateRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  const BSTree< int, std::string >& constTree = tree;
  BSTree< int, std::string >::const_iterator it = constTree.find(3);
  tree.rotateLargeLeft(it);

  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");

  const BSTree< int, std::string >& constTree = tree;
  BSTree< int, std::string >::const_iterator it = constTree.find(1);
  tree.rotateLargeRight(it);

  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(rotate_on_invalid_node)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BSTree< int, std::string >::const_iterator it = tree.cend();
  tree.rotateLeft(it);
  tree.rotateRight(it);
  tree.rotateLargeLeft(it);
  tree.rotateLargeRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
}

BOOST_AUTO_TEST_CASE(clear)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  tree.clear();

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(clear_on_empty_tree)
{
  BSTree< int, std::string > tree;
  tree.clear();

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
}

BOOST_AUTO_TEST_CASE(works_with_string_keys)
{
  BSTree< std::string, int > tree;
  tree.push("ten", 10);
  tree.push("twenty", 20);

  BOOST_CHECK_EQUAL(tree.get("ten"), 10);
  BOOST_CHECK_EQUAL(tree.get("twenty"), 20);
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_CASE(works_with_double_keys)
{
  BSTree< double, std::string > tree;
  tree.push(1.5, "one point five");
  tree.push(2.7, "two point seven");

  BOOST_CHECK_EQUAL(tree.get(1.5), "one point five");
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
