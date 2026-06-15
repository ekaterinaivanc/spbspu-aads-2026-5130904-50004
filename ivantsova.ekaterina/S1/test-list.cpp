#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include <list.hpp>

BOOST_AUTO_TEST_CASE(push_pop)
{
  ivantsova::List< int > list;
  BOOST_CHECK(list.empty());

  list.push_back(42);
  list.push_front(13);

  BOOST_CHECK_EQUAL(list.front(), 13);
  BOOST_CHECK_EQUAL(list.back(), 42);
  BOOST_CHECK_EQUAL(list.size(), 2);

  list.pop_front();
  BOOST_CHECK_EQUAL(list.front(), 42);
  list.pop_back();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(copy_move)
{
  ivantsova::List< int > list1;
  list1.push_back(7);
  list1.push_back(8);

  ivantsova::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.front(), 7);
  BOOST_CHECK_EQUAL(list2.back(), 8);

  ivantsova::List< int > list3(std::move(list1));
  BOOST_CHECK(list1.empty());
  BOOST_CHECK_EQUAL(list3.front(), 7);
}
