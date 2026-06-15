#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(stack_push_and_pop)
{
  ivantsova::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  BOOST_CHECK_EQUAL(stack.pop(), 3);
  BOOST_CHECK_EQUAL(stack.pop(), 2);
  BOOST_CHECK_EQUAL(stack.pop(), 1);
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(stack_pop_empty)
{
  ivantsova::Stack< int > stack;
  BOOST_CHECK_THROW(stack.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(stack_top_empty)
{
  ivantsova::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
