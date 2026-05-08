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
#include <stddef.h>
#include <string.h>
#include <test/base.h>

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

	qr_module_set(qr, qr->side_length - 1, qr->side_length - 1, QR_MODULE_LIGHT);
	test_expect_eq(qr_module_get(qr, qr->side_length - 1, qr->side_length - 1), QR_MODULE_LIGHT,
		"Last module should be set to light");

	qr_destroy(qr);

	return TEST_SUCCESS;
}

/**
 * @brief Test reserved module detection across different versions
 */
TEST(reserved_module_detection_version1)
{
	size_t i, j;
	unsigned reserved = 0;

	// Test version 1 (no alignment patterns, no version info)
	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code v1");

	// Finder patterns + separators
	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			test_expect_eq(qr_module_is_reserved(qr, i, j), 1, "Upper left finder pattern");
			test_expect_eq(qr_module_is_reserved(qr, i, qr->side_length - j + 1), 1, "Upper right finder pattern");
			test_expect_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, j), 1, "Lower left finder pattern");
		}
	}

	// Timing pattern
	for (i = 8; i < 13; ++i)
	{
		test_expect_eq(qr_module_is_reserved(qr, 6, i), 1, "Horizontal timing pattern");
		test_expect_eq(qr_module_is_reserved(qr, i, 6), 1, "Vertical timing pattern");
	}

	// Format info
	for (i = 0; i < 8; ++i)
	{
		test_expect_eq(qr_module_is_reserved(qr, 8, i), 1, "Upper left format info MSB");
		test_expect_eq(qr_module_is_reserved(qr, i, 8), 1, "Upper left format info LSB");
		test_expect_eq(qr_module_is_reserved(qr, 8, qr->side_length - i + 1), 1, "Upper right format info LSB");
		test_expect_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, 8), 1, "Lower left format info MSB");
	}

	// Version info (should NOT be reserved for v1)
	test_expect_eq(qr_module_is_reserved(qr, 0, qr->side_length - 9), 0, "Version info reserved");

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (qr_module_is_reserved(qr, i, j))
				++reserved;
		}
	}

	test_expect_eq(reserved, 233, "Total number of reserved modules");

	qr_destroy(qr);

	return TEST_SUCCESS;
}

TEST(reserved_module_detection_version7)
{
	size_t i, j;
	unsigned reserved = 0;

	qr_code *qr = qr_create(7, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code v7");

	// Finder patterns + separators
	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			test_expect_eq(qr_module_is_reserved(qr, i, j), 1, "Upper left finder pattern");
			test_expect_eq(qr_module_is_reserved(qr, i, qr->side_length - j + 1), 1, "Upper right finder pattern");
			test_expect_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, j), 1, "Lower left finder pattern");
		}
	}

	// Timing pattern
	for (i = 8; i < 13; ++i)
	{
		test_expect_eq(qr_module_is_reserved(qr, 6, i), 1, "Horizontal timing pattern");
		test_expect_eq(qr_module_is_reserved(qr, i, 6), 1, "Vertical timing pattern");
	}

	// Format info
	for (i = 0; i < 8; ++i)
	{
		test_expect_eq(qr_module_is_reserved(qr, 8, i), 1, "Upper left format info MSB");
		test_expect_eq(qr_module_is_reserved(qr, i, 8), 1, "Upper left format info LSB");
		test_expect_eq(qr_module_is_reserved(qr, 8, qr->side_length - i + 1), 1, "Upper right format info LSB");
		test_expect_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, 8), 1, "Lower left format info MSB");
	}

	// Version info
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 6; ++j)
		{
			test_expect_eq(qr_module_is_reserved(qr, j, qr->side_length - i - 9), 1, "Upper version info");
			test_expect_eq(qr_module_is_reserved(qr, qr->side_length - i - 9, j), 1, "Lower version info");
		}
	}

	// Alignment patterns
	for (i = 0; i < 5; ++i)
	{
		for (j = 0; j < 5; ++j)
		{
			test_expect_eq(qr_module_is_reserved(qr, 4 + i, 20 + j), 1, "Alignment pattern 1");
			test_expect_eq(qr_module_is_reserved(qr, 20 + i, 4 + j), 1, "Alignment pattern 2");
			test_expect_eq(qr_module_is_reserved(qr, 20 + i, 20 + j), 1, "Alignment pattern 3");
			test_expect_eq(qr_module_is_reserved(qr, 20 + i, 36 + j), 1, "Alignment pattern 4");
			test_expect_eq(qr_module_is_reserved(qr, 36 + i, 20 + j), 1, "Alignment pattern 5");
			test_expect_eq(qr_module_is_reserved(qr, 36 + i, 36 + j), 1, "Alignment pattern 6");
		}
	}

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (qr_module_is_reserved(qr, i, j))
				++reserved;
		}
	}

	test_expect_eq(reserved, 457, "Total number of reserved modules");

	qr_destroy(qr);

	return TEST_SUCCESS;
}

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

TEST(codeword_placement_wrap_version1)
{
	size_t i;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	for (i = 0; i < qr->codeword_count; ++i)
		qr->codewords[i] = (word) i;

	qr_place_codewords(qr);

	// first codeword (0)
	test_expect_eq(qr_module_get(qr, 20, 20), QR_MODULE_LIGHT, "First codeword (0) bit 8");
	test_expect_eq(qr_module_get(qr, 20, 19), QR_MODULE_LIGHT, "First codeword (0) bit 7");
	test_expect_eq(qr_module_get(qr, 19, 20), QR_MODULE_LIGHT, "First codeword (0) bit 6");
	test_expect_eq(qr_module_get(qr, 19, 19), QR_MODULE_LIGHT, "First codeword (0) bit 5");
	test_expect_eq(qr_module_get(qr, 18, 20), QR_MODULE_LIGHT, "First codeword (0) bit 4");
	test_expect_eq(qr_module_get(qr, 18, 19), QR_MODULE_LIGHT, "First codeword (0) bit 3");
	test_expect_eq(qr_module_get(qr, 17, 20), QR_MODULE_LIGHT, "First codeword (0) bit 2");
	test_expect_eq(qr_module_get(qr, 17, 19), QR_MODULE_LIGHT, "First codeword (0) bit 1");

	// 13. codeword (12=8+4)
	test_expect_eq(qr_module_get(qr, 20, 12), QR_MODULE_LIGHT, "13. codeword (12) bit 8");
	test_expect_eq(qr_module_get(qr, 20, 11), QR_MODULE_LIGHT, "13. codeword (12) bit 7");
	test_expect_eq(qr_module_get(qr, 19, 12), QR_MODULE_LIGHT, "13. codeword (12) bit 6");
	test_expect_eq(qr_module_get(qr, 19, 11), QR_MODULE_LIGHT, "13. codeword (12) bit 5");
	test_expect_eq(qr_module_get(qr, 18, 12), QR_MODULE_DARK, "13. codeword (12) bit 4");
	test_expect_eq(qr_module_get(qr, 18, 11), QR_MODULE_DARK, "13. codeword (12) bit 3");
	test_expect_eq(qr_module_get(qr, 17, 11), QR_MODULE_LIGHT, "13. codeword (12) bit 2");
	test_expect_eq(qr_module_get(qr, 17, 12), QR_MODULE_LIGHT, "13. codeword (12) bit 1");

	// 19. codeword (18=16+2)
	test_expect_eq(qr_module_get(qr, 4, 10), QR_MODULE_LIGHT, "19. codeword (18) bit 8");
	test_expect_eq(qr_module_get(qr, 4, 9), QR_MODULE_LIGHT, "19. codeword (18) bit 7");
	test_expect_eq(qr_module_get(qr, 5, 10), QR_MODULE_LIGHT, "19. codeword (18) bit 6");
	test_expect_eq(qr_module_get(qr, 5, 9), QR_MODULE_DARK, "19. codeword (18) bit 5");
	test_expect_eq(qr_module_get(qr, 7, 10), QR_MODULE_LIGHT, "19. codeword (18) bit 4");
	test_expect_eq(qr_module_get(qr, 7, 9), QR_MODULE_LIGHT, "19. codeword (18) bit 3");
	test_expect_eq(qr_module_get(qr, 8, 10), QR_MODULE_DARK, "19. codeword (18) bit 2");
	test_expect_eq(qr_module_get(qr, 8, 9), QR_MODULE_LIGHT, "19. codeword (18) bit 1");

	qr_destroy(qr);

	return TEST_SUCCESS;
}

TEST(codeword_placement_wrap_version21)
{
	size_t i;

	qr_code *qr = qr_create(21, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	for (i = 0; i < qr->codeword_count; ++i)
		qr->codewords[i] = (word) i;

	qr_place_codewords(qr);

	// 56. codeword (55=32+16+4+2+1)
	test_expect_eq(qr_module_get(qr, 54, 96), QR_MODULE_LIGHT, "56. codeword (55) bit 8");
	test_expect_eq(qr_module_get(qr, 54, 95), QR_MODULE_LIGHT, "56. codeword (55) bit 7");
	test_expect_eq(qr_module_get(qr, 53, 96), QR_MODULE_DARK, "56. codeword (55) bit 6");
	test_expect_eq(qr_module_get(qr, 53, 95), QR_MODULE_DARK, "56. codeword (55) bit 5");
	test_expect_eq(qr_module_get(qr, 47, 96), QR_MODULE_LIGHT, "56. codeword (55) bit 4");
	test_expect_eq(qr_module_get(qr, 47, 95), QR_MODULE_DARK, "56. codeword (55) bit 3");
	test_expect_eq(qr_module_get(qr, 46, 96), QR_MODULE_DARK, "56. codeword (55) bit 2");
	test_expect_eq(qr_module_get(qr, 46, 95), QR_MODULE_DARK, "56. codeword (55) bit 1");

	// 104. codeword (103=64+32+4+2+1)
	test_expect_eq(qr_module_get(qr, 8, 90), QR_MODULE_LIGHT, "104. codeword (103) bit 8");
	test_expect_eq(qr_module_get(qr, 8, 89), QR_MODULE_DARK, "104. codeword (103) bit 7");
	test_expect_eq(qr_module_get(qr, 9, 90), QR_MODULE_DARK, "104. codeword (103) bit 6");
	test_expect_eq(qr_module_get(qr, 9, 89), QR_MODULE_LIGHT, "104. codeword (103) bit 5");
	test_expect_eq(qr_module_get(qr, 10, 90), QR_MODULE_LIGHT, "104. codeword (103) bit 4");
	test_expect_eq(qr_module_get(qr, 10, 89), QR_MODULE_DARK, "104. codeword (103) bit 3");
	test_expect_eq(qr_module_get(qr, 11, 90), QR_MODULE_DARK, "104. codeword (103) bit 2");
	test_expect_eq(qr_module_get(qr, 11, 89), QR_MODULE_DARK, "104. codeword (103) bit 1");

	// 200. codeword (199=128+64+4+2+1)
	test_expect_eq(qr_module_get(qr, 9, 84), QR_MODULE_DARK, "200. codeword (199) bit 8");
	test_expect_eq(qr_module_get(qr, 9, 83), QR_MODULE_DARK, "200. codeword (199) bit 7");
	test_expect_eq(qr_module_get(qr, 8, 84), QR_MODULE_LIGHT, "200. codeword (199) bit 6");
	test_expect_eq(qr_module_get(qr, 8, 83), QR_MODULE_LIGHT, "200. codeword (199) bit 5");
	test_expect_eq(qr_module_get(qr, 7, 84), QR_MODULE_LIGHT, "200. codeword (199) bit 4");
	test_expect_eq(qr_module_get(qr, 7, 83), QR_MODULE_DARK, "200. codeword (199) bit 3");
	test_expect_eq(qr_module_get(qr, 5, 84), QR_MODULE_DARK, "200. codeword (199) bit 2");
	test_expect_eq(qr_module_get(qr, 5, 83), QR_MODULE_DARK, "200. codeword (199) bit 1");

	qr_destroy(qr);

	return TEST_SUCCESS;
}
