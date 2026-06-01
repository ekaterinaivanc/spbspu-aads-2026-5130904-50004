#include <boost/test/unit_test.hpp>
#include "operations.hpp"

BOOST_AUTO_TEST_SUITE(ShiftTests)

BOOST_AUTO_TEST_CASE(shift_left)
{
  BOOST_CHECK_EQUAL(ivantsova::calculateExpression("1 << 2"), 4);
  BOOST_CHECK_EQUAL(ivantsova::calculateExpression("5 << 1"), 10);
  BOOST_CHECK_EQUAL(ivantsova::calculateExpression("10 << 2"), 40);
}

BOOST_AUTO_TEST_CASE(shift_left_with_parentheses)
{
  BOOST_CHECK_EQUAL(ivantsova::calculateExpression("( 1 << 2 ) + 3"), 7);
}

BOOST_AUTO_TEST_CASE(shift_left_negative_number)
{
  BOOST_CHECK_THROW(ivantsova::calculateExpression("5 << -1"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(shift_left_overflow)
{
  BOOST_CHECK_THROW(ivantsova::calculateExpression("1 << 100"), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
