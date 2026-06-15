#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(queue_push_and_pop)
{
  ivantsova::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.front(), 1);
  BOOST_CHECK_EQUAL(queue.pop(), 1);
  BOOST_CHECK_EQUAL(queue.front(), 2);
  BOOST_CHECK_EQUAL(queue.pop(), 2);
  BOOST_CHECK_EQUAL(queue.front(), 3);
  BOOST_CHECK_EQUAL(queue.pop(), 3);
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(queue_pop_empty)
{
  ivantsova::Queue< int > queue;
  BOOST_CHECK_THROW(queue.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(queue_front_empty)
{
  ivantsova::Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
