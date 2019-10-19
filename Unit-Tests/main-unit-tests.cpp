#include "pch-unit-tests.hpp"
#define BOOST_TEST_MODULE IStream_Extra Test Suite
#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(main_suite, *utf::enabled())

BOOST_AUTO_TEST_CASE(ensure_64_bit) //
{
	BOOST_REQUIRE_EQUAL(sizeof(void*), 8);
}

BOOST_AUTO_TEST_SUITE_END()
