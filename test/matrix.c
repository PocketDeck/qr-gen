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
 * @brief Test whether codeword placement respects reserved modules
 *
 * Verifies that when all codeword bits are 1, all non-reserved modules
 * are set to QR_MODULE_DARK after placement.
 */
TEST(codeword_placement_reserved)
{
	size_t i, j;

	// Create test QR code, version 1
	qr_code *qr = qr_create(0, QR_MODE_BYTE, QR_EC_LEVEL_H);
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
	qr_code *qr = qr_create(0, QR_MODE_BYTE, QR_EC_LEVEL_H);
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

	return TEST_SUCCESS;
}

/**
 * @brief Test reserved module detection
 *
 * Verifies that reserved modules (finder patterns, timing patterns, etc.)
 * are correctly identified.
 */
TEST(reserved_module_detection)
{
	size_t i, j;

	// Create test QR code, version 1
	qr_code *qr = qr_create(0, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Test finder pattern positions (top-left corner)
	for (i = 0; i < 7; ++i)
	{
		for (j = 0; j < 7; ++j)
		{
			test_expect_eq(qr_module_is_reserved(qr, i, j), 1,
				"Finder pattern position should be reserved");
		}
	}

	// Test timing pattern (row 6, columns 8-12)
	for (j = 8; j < 13; ++j)
	{
		test_expect_eq(qr_module_is_reserved(qr, 6, j), 1,
			"Timing pattern position should be reserved");
	}

	// Test format information (top-left corner)
	test_expect_eq(qr_module_is_reserved(qr, 8, 8), 1,
		"Format information module should be reserved");

	// Test non-reserved area (outside finder patterns and timing patterns)
	test_expect_eq(qr_module_is_reserved(qr, 9, 8), 0,
		"Data module should not be reserved");

	return TEST_SUCCESS;
}

/**
 * @brief Test edge cases for module access
 *
 * Verifies that the module access functions handle edge cases correctly.
 */
TEST(module_edge_cases)
{
	// Create test QR code, version 1
	qr_code *qr = qr_create(0, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Test setting and getting the last module
	qr_module_set(qr, qr->side_length - 1, qr->side_length - 1, QR_MODULE_DARK);
	test_expect_eq(qr_module_get(qr, qr->side_length - 1, qr->side_length - 1), QR_MODULE_DARK,
		"Last module should be set to dark");

	return TEST_SUCCESS;
}
