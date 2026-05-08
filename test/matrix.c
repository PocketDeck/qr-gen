/**
 * @file matrix.c
 * @brief Test cases for QR code matrix operations
 *
 * This file contains test cases for the QR code matrix module, including
 * module manipulation, reserved module detection, and codeword placement.
 */

#include <qr/matrix.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <string.h>
#include <test/base.h>

/**
 * @brief Test whether codeword placement respects reserved modules across multiple versions
 *
 * Verifies that when all codeword bits are 1, all non-reserved modules
 * are set to QR_MODULE_DARK after placement.
 */
TEST(codeword_placement_reserved)
{
	size_t i, j, v;
	const unsigned versions[] = { 1, 2, 7, 8, 40 }; // Test versions 1, 2, 7, 8, 40

	for (v = 0; v < sizeof(versions) / sizeof(versions[0]); ++v)
	{
		qr_code *qr = qr_create(versions[v], QR_MODE_BYTE, QR_EC_LEVEL_H);
		if (!qr) return TEST_FAILURE("Failed to create test QR code");

		// Initialize all matrix modules to QR_MODULE_LIGHT (0)
		memset(qr->matrix, 0, qr->side_length * qr->side_length * sizeof(*qr->matrix));

		// Set all codeword bits to 1
		memset(qr->codewords, 0xFF, qr->codeword_count * sizeof(word));

		// Place codewords in the matrix
		qr_place_codewords(qr);

		// Verify the results
		for (i = 0; i < qr->side_length; ++i)
		{
			for (j = 0; j < qr->side_length; ++j)
			{
				if (qr_module_is_reserved(qr, i, j))
				{
					// Reserved modules should remain QR_MODULE_LIGHT (0)
					test_expect_eq(qr_module_get(qr, i, j), QR_MODULE_LIGHT,
						"Reserved module should remain light");
				}
				else
				{
					// Non-reserved modules should be QR_MODULE_DARK (1)
					test_expect_eq(qr_module_get(qr, i, j), QR_MODULE_DARK,
						"Non-reserved module should be set to dark");
				}
			}
		}

		qr_destroy(qr);
	}

	return TEST_SUCCESS;
}

/**
 * @brief Test module get/set operations
 *
 * Verifies that module states can be set and retrieved correctly.
 */
TEST(module_get_set)
{
	size_t i, j;
	qr_module_state actual, expected;

	// Create test QR code, version 1
	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Test setting and getting module states
	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			// Alternate between dark and light modules
			expected = (i + j) % 2 ? QR_MODULE_DARK : QR_MODULE_LIGHT;
			qr_module_set(qr, i, j, expected);
			actual = qr_module_get(qr, i, j);

			test_expect_eq(actual, expected,
				"Module state should match expected value");
		}
	}
	qr_destroy(qr);

	return TEST_SUCCESS;
}

/**
 * @brief Test reserved module detection across different versions
 */
TEST(reserved_module_detection)
{
	size_t i, j;

	// Test version 1 (no alignment patterns, no version info)
	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code v1");

	// Finder patterns
	for (i = 0; i < 7; ++i)
		for (j = 0; j < 7; ++j)
			test_expect_eq(qr_module_is_reserved(qr, i, j), 1, "Finder pattern reserved");

	// Timing pattern
	for (j = 8; j < 13; ++j)
		test_expect_eq(qr_module_is_reserved(qr, 6, j), 1, "Timing pattern reserved");

	// Format info
	test_expect_eq(qr_module_is_reserved(qr, 8, 8), 1, "Format info reserved");

	// Version info (should NOT be reserved for v1)
	test_expect_eq(qr_module_is_reserved(qr, 0, qr->side_length - 11), 0, "Version info not reserved v1");
	qr_destroy(qr);

	// Test version 7 (has alignment patterns and version info)
	qr = qr_create(7, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code v7");

	// Version info (should be reserved for v7+)
	// Lower-left version info: (side-11, 0) to (side-9, 5)
	test_expect_eq(qr_module_is_reserved(qr, qr->side_length - 11, 0), 1, "Version info reserved v7 LL");
	test_expect_eq(qr_module_is_reserved(qr, qr->side_length - 9, 5), 1, "Version info reserved v7 LL");

	// Upper-right version info: (0, side-11) to (5, side-9)
	test_expect_eq(qr_module_is_reserved(qr, 0, qr->side_length - 11), 1, "Version info reserved v7 UR");
	test_expect_eq(qr_module_is_reserved(qr, 5, qr->side_length - 9), 1, "Version info reserved v7 UR");

	// Alignment patterns (for v7, centers are at 6, 22, 38 - side_length is 45)
	test_expect_eq(qr_module_is_reserved(qr, 22, 22), 1, "Alignment pattern center reserved v7");
	test_expect_eq(qr_module_is_reserved(qr, 20, 20), 1, "Alignment pattern corner reserved v7");
	test_expect_eq(qr_module_is_reserved(qr, 24, 24), 1, "Alignment pattern corner reserved v7");
	qr_destroy(qr);

	return TEST_SUCCESS;
}

/**
 * @brief Test edge cases for module access
 */
TEST(module_edge_cases)
{
	// Create test QR code, version 1
	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Test setting and getting the last module
	qr_module_set(qr, qr->side_length - 1, qr->side_length - 1, QR_MODULE_DARK);
	test_expect_eq(qr_module_get(qr, qr->side_length - 1, qr->side_length - 1), QR_MODULE_DARK,
		"Last module should be set to dark");
	qr_destroy(qr);

	return TEST_SUCCESS;
}
