#include <test/base.h>

/**
 * Basic sanity check test to verify the test framework is working.
 * This is a simple test that verifies basic arithmetic works as expected.
 * 
 * @return TEST_SUCCESS if the test passes, error message if it fails
 */
TEST(sanity_addition)
{
	assert_equal(3 + 4, 7, "Basic addition failed");
	return TEST_SUCCESS;
}
