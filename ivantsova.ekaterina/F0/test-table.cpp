#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "hash_table.hpp"

BOOST_AUTO_TEST_SUITE(DoubleHashTable_tests)

BOOST_AUTO_TEST_CASE(test_constructor_default)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  BOOST_CHECK_EQUAL(table.getCount(), 0);
  BOOST_CHECK(table.getSize() >= 101);
}

BOOST_AUTO_TEST_CASE(test_constructor)
{
  ivantsova::DoubleHashTable< std::string, int > table(50);
  BOOST_CHECK_EQUAL(table.getCount(), 0);
  BOOST_CHECK(table.getSize() >= 53);
}

BOOST_AUTO_TEST_CASE(test_insert)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  BOOST_CHECK(table.insert("key1", 10));
  BOOST_CHECK(table.insert("key2", 20));
  BOOST_CHECK_EQUAL(table.getCount(), 2);
}

BOOST_AUTO_TEST_CASE(test_insert_duplicate)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  BOOST_CHECK(table.insert("key1", 10));
  BOOST_CHECK(!table.insert("key1", 20));
  BOOST_CHECK_EQUAL(table.getCount(), 1);
}

BOOST_AUTO_TEST_CASE(test_find)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);
  table.insert("key2", 20);

  auto it = table.find("key1");
  BOOST_CHECK(it != table.end());
  BOOST_CHECK_EQUAL(it->value, 10);

  it = table.find("key3");
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(test_find_const)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);

  const auto& constTable = table;
  auto it = constTable.find("key1");
  BOOST_CHECK(it != constTable.end());
  BOOST_CHECK_EQUAL(it->value, 10);
}

BOOST_AUTO_TEST_CASE(test_findExists)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);

  BOOST_CHECK(table.findExists("key1"));
  BOOST_CHECK(!table.findExists("key2"));
}


BOOST_AUTO_TEST_CASE(test_remove)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);
  table.insert("key2", 20);

  BOOST_CHECK(table.remove("key1"));
  BOOST_CHECK_EQUAL(table.getCount(), 1);
  BOOST_CHECK(!table.findExists("key1"));
  BOOST_CHECK(table.findExists("key2"));
}

BOOST_AUTO_TEST_CASE(test_remove_non_existent)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);

  BOOST_CHECK(!table.remove("key2"));
  BOOST_CHECK_EQUAL(table.getCount(), 1);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  ivantsova::DoubleHashTable< std::string, int > table1;
  table1.insert("key1", 10);
  table1.insert("key2", 20);

  ivantsova::DoubleHashTable< std::string, int > table2(table1);

  BOOST_CHECK_EQUAL(table2.getCount(), 2);
  BOOST_CHECK(table2.findExists("key1"));
  BOOST_CHECK(table2.findExists("key2"));

  auto it = table2.find("key1");
  BOOST_CHECK_EQUAL(it->value, 10);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor_empty)
{
  ivantsova::DoubleHashTable< std::string, int > table1;
  ivantsova::DoubleHashTable< std::string, int > table2(table1);

  BOOST_CHECK_EQUAL(table2.getCount(), 0);
}

BOOST_AUTO_TEST_CASE(test_assignment_operator)
{
  ivantsova::DoubleHashTable< std::string, int > table1;
  table1.insert("key1", 10);
  table1.insert("key2", 20);

  ivantsova::DoubleHashTable< std::string, int > table2;
  table2 = table1;

  BOOST_CHECK_EQUAL(table2.getCount(), 2);
  BOOST_CHECK(table2.findExists("key1"));
  BOOST_CHECK(table2.findExists("key2"));
}

BOOST_AUTO_TEST_CASE(test_assignment_self)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);

  table = table;

  BOOST_CHECK_EQUAL(table.getCount(), 1);
  BOOST_CHECK(table.findExists("key1"));
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  ivantsova::DoubleHashTable< std::string, int > table1;
  table1.insert("key1", 10);

  ivantsova::DoubleHashTable< std::string, int > table2;
  table2.insert("key2", 20);

  table1.swap(table2);

  BOOST_CHECK(table1.findExists("key2"));
  BOOST_CHECK(!table1.findExists("key1"));
  BOOST_CHECK(table2.findExists("key1"));
  BOOST_CHECK(!table2.findExists("key2"));
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);
  table.insert("key2", 20);
  table.insert("key3", 30);

  size_t oldSize = table.getSize();
  table.clear();

  BOOST_CHECK_EQUAL(table.getCount(), 0);
  BOOST_CHECK_EQUAL(table.getSize(), oldSize);
  BOOST_CHECK(!table.findExists("key1"));
}

BOOST_AUTO_TEST_CASE(test_clear_empty)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  size_t oldSize = table.getSize();
  table.clear();

  BOOST_CHECK_EQUAL(table.getCount(), 0);
  BOOST_CHECK_EQUAL(table.getSize(), oldSize);
}

BOOST_AUTO_TEST_CASE(test_iterator_begin_end)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);
  table.insert("key2", 20);

  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);

  const auto& constTable = table;
  int count = 0;
  for (auto it = constTable.begin(); it != constTable.end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 1);
}

BOOST_AUTO_TEST_CASE(test_iterator_operator_star)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);

  auto it = table.find("key1");
  BOOST_CHECK_EQUAL((*it).value, 10);
  BOOST_CHECK_EQUAL(it->value, 10);
}

BOOST_AUTO_TEST_CASE(test_getAllKeys)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  table.insert("key1", 10);
  table.insert("key2", 20);
  table.insert("key3", 30);

  ivantsova::MyVector< std::string > keys;
  table.getAllKeys(keys);

  BOOST_CHECK_EQUAL(keys.size(), 3);

  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
  for (size_t i = 0; i < keys.size(); ++i) {
    if (keys[i] == "key1") hasKey1 = true;
    if (keys[i] == "key2") hasKey2 = true;
    if (keys[i] == "key3") hasKey3 = true;
  }
  BOOST_CHECK(hasKey1);
  BOOST_CHECK(hasKey2);
  BOOST_CHECK(hasKey3);
}

BOOST_AUTO_TEST_CASE(test_getCount)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  BOOST_CHECK_EQUAL(table.getCount(), 0);

  table.insert("key1", 10);
  BOOST_CHECK_EQUAL(table.getCount(), 1);

  table.insert("key2", 20);
  BOOST_CHECK_EQUAL(table.getCount(), 2);

  table.remove("key1");
  BOOST_CHECK_EQUAL(table.getCount(), 1);
}

BOOST_AUTO_TEST_CASE(test_getSize)
{
  ivantsova::DoubleHashTable< std::string, int > table(10);
  size_t size1 = table.getSize();
  BOOST_CHECK(size1 >= 11);

  for (int i = 0; i < 50; ++i) {
    table.insert("key" + std::to_string(i), i);
  }

  size_t size2 = table.getSize();
  BOOST_CHECK(size2 > size1);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  ivantsova::DoubleHashTable< std::string, int > table(10);
  size_t oldSize = table.getSize();

  for (int i = 0; i < 20; ++i) {
    table.insert("key" + std::to_string(i), i);
  }

  BOOST_CHECK(table.getSize() > oldSize);
  BOOST_CHECK_EQUAL(table.getCount(), 20);

  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.findExists("key" + std::to_string(i)));
  }
}

BOOST_AUTO_TEST_CASE(test_rehash_with_duplicates)
{
  ivantsova::DoubleHashTable< std::string, int > table(10);

  for (int i = 0; i < 20; ++i) {
    table.insert("key" + std::to_string(i), i);
  }

  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(!table.insert("key" + std::to_string(i), i + 100));
  }

  BOOST_CHECK_EQUAL(table.getCount(), 20);
}

BOOST_AUTO_TEST_CASE(test_empty_table)
{
  ivantsova::DoubleHashTable< std::string, int > table;
  auto it = table.find("key");
  BOOST_CHECK(it == table.end());
  BOOST_CHECK(!table.remove("key"));
  BOOST_CHECK_EQUAL(table.getCount(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
