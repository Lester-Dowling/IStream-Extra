// Unit-Tests/test-pair.cpp
#include "pch-unit-tests.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include "IStream-Extra/pair.hpp"
#include "put-to-operators.hpp"

BOOST_AUTO_TEST_SUITE(test_pair_suite, *boost::unit_test::enabled())

/*
 * Unit tests to verify the correct operation of pair input.
 */

typedef boost::mpl::list<int16_t, int32_t, float, double> signed_test_types;
typedef boost::mpl::list<uint16_t, uint32_t> unsigned_test_types;
typedef boost::mpl::list<int16_t, uint16_t, int32_t, uint32_t, float, double>
  common_number_test_types;
typedef boost::mpl::list<char, unsigned char, signed char> char_test_types;
typedef boost::mpl::list<float, double> real_test_types;

BOOST_AUTO_TEST_CASE(see_what_happens_when_std_parses_a_list_of_doubles)
{
	using namespace std;
	istringstream doubles{ "   9.8   1.9   0.3   8.1  78.99     " };
	vector<double> expected_answer = { 9.8, 1.9, 0.3, 8.1, 78.99 };
	vector<double> vd;
	vd.reserve(10);
	try {
		doubles.exceptions(ios_base::badbit | ios_base::failbit);
		vd.insert(
		  vd.end(), istream_iterator<double>{ doubles }, std::istream_iterator<double>{});
	}
	catch (const std::ios_base::failure&) {
		if (doubles.fail()) {
			// BOOST_TEST_MESSAGE("doubles has failbit set");
		}
		if (doubles.bad()) {
			// BOOST_TEST_MESSAGE("doubles has badbit set");
			BOOST_FAIL("Failed to parse doubles");
		}
		if (doubles.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("doubles has eofbit set");
			BOOST_TEST(vd.size() == 5);
			return;
		}
		else
			BOOST_FAIL("Failed to parse doubles");
	}
	BOOST_REQUIRE(vd == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(input_of_signed_number_pairs, T, signed_test_types)
{
	using namespace std;
	typedef pair<T, T> Pair;
	istringstream iss{ "  (-9,8)   (1,9)   (0,3)   (-8,1)   (78,99) " };
	vector<Pair> expected_answer = { { -9, 8 }, { 1, 9 }, { 0, 3 }, { -8, 1 }, { 78, 99 } };

	// Read iss into v:
	vector<Pair> v;
	try {
		iss.exceptions(ios_base::badbit);
		v.insert(v.end(), istream_iterator<Pair>{ iss }, istream_iterator<Pair>{});
	}
	catch (const ios_base::failure& e__) {
		// The exception msg when EOF:
		const string eof_description("basic_ios::clear"), exception_msg(e__.what());
		// If this exception is not the result of EOF, then re-throw it:
		if (exception_msg.find(eof_description) == string::npos)
			throw;
		// Else, the exception was thrown as a result of EOF which is normal. Okay to
		// continue.
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(input_of_unsigned_numbers_pairs, T, unsigned_test_types)
{
	using namespace std;
	typedef pair<T, T> Pair;
	istringstream iss("   (9,8)   (1,9)   (0,3)   (8,1)  (78,99)     ");
	vector<Pair> expected_answer = { { 9, 8 }, { 1, 9 }, { 0, 3 }, { 8, 1 }, { 78, 99 } };
	vector<Pair> v;
	v.reserve(10);
	try {
		iss.exceptions(ios_base::badbit | ios_base::failbit);
		v.insert(v.end(), istream_iterator<Pair>{ iss }, std::istream_iterator<Pair>{});
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 5);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(input_of_char_pairs, T, char_test_types)
{
	using namespace std;
	typedef pair<T, T> Pair;
	istringstream iss("(a,b) ( A , B ) (c,d) (e,f) (g,h)");
	vector<Pair> expected_answer = {
		{ 'a', 'b' }, { 'A', 'B' }, { 'c', 'd' }, { 'e', 'f' }, { 'g', 'h' }
	};
	// Read iss into v:
	vector<Pair> v;
	try {
		iss.exceptions(ios_base::badbit | ios_base::failbit);
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 5);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(input_of_real_number_pairs, T, real_test_types)
{
	using namespace std;
	typedef pair<T, T> Pair;
	istringstream iss("   ( 3.55 , 373.23)    ( 2.4 , 1.5)    ( 5.9 , -1e10 )    ");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> expected_answer = { { 3.55, 373.23 }, { 2.4, 1.5 }, { 5.9, -1e10 } };

	// Read iss into v:
	vector<Pair> v;
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
  input_of_one_word_string_and_real_number_pair,
  T,
  real_test_types)
{
	using namespace std;
	typedef pair<string, T> Pair;
	istringstream iss("   ( abc , 373.23)    ( def , 1.5)    ( ghi , -1e10 )    ");
	vector<Pair> expected_answer = { { "abc", 373.23 }, { "def", 1.5 }, { "ghi", -1e10 } };

	// Read iss into v:
	vector<Pair> v;
	try {
		iss.exceptions(ios_base::badbit | ios_base::failbit);
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
  input_of_multi_word_string_and_real_number_pair,
  T,
  real_test_types)
{
	using namespace std;
	typedef pair<string, T> Pair;
	istringstream iss(
	  "   ( 'abc \"def' , 373.23)    ( \\\"def\\\" , 1.5)    ( ghi , -1e10 )    ");
	vector<Pair> expected_answer = { { "abc \"def", 373.23 },
									 { "\"def\"", 1.5 },
									 { "ghi", -1e10 } };
	// Read iss into v:
	vector<Pair> v;
	try {
		iss.exceptions(ios_base::badbit | ios_base::failbit);
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
  input_of_real_numbers_and_multi_word_string_pair,
  T,
  real_test_types)
{
	using namespace std;
	typedef pair<T, string> Pair;
	istringstream iss(
	  "   ( 373.23 , 'abc \"123' )    ( 1.5 , \\\"def\\\"  )    ( -1e10 , ghi )   ");
	vector<Pair> expected_answer = { { 373.23, "abc \"123" },
									 { 1.5, "\"def\"" },
									 { -1e10, "ghi" } };
	// Read iss into v:
	vector<Pair> v;
	try {
		iss.exceptions(ios_base::badbit | ios_base::failbit);
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE(v == expected_answer);
}

BOOST_AUTO_TEST_CASE(input_of_strings_and_comment_pair)
{
	using namespace std;
	typedef pair<string, string> Pair;
	istringstream iss("   ( \"xyz\" , 'abc \"123' )    ( lmn , def )    ( /* in a comment "
					  "*/ qrs , ghi )    ");
	vector<Pair> expected_answer = { { "xyz", "abc \"123" },
									 { "lmn", "def" },
									 { "qrs", "ghi" } };
	vector<Pair> v;
	try {
		iss.exceptions(ios_base::badbit | ios_base::failbit);
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}
	BOOST_REQUIRE_EQUAL(v, expected_answer);
}


BOOST_AUTO_TEST_SUITE_END();
