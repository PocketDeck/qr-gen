/**
 * @file matrix.c
 * @brief QR code matrix operations tests
 */

#include <qr/matrix.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stddef.h>
#include <string.h>
#include <test/base.h>

/**
 * @brief Test module get/set operations
 */
TEST(module_get_set)
{
	size_t i, j;
	qr_module actual, expected;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Test setting and getting modules in checkerboard pattern
	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			expected = (i + j) % 2 ? QR_MODULE_DARK : QR_MODULE_LIGHT;
			qr_module_set(qr, i, j, expected);
			actual = qr_module_get(qr, i, j);

			test_eq(actual, expected,
				"Module state incorrect");
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
	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	qr_module_set(qr, qr->side_length - 1, qr->side_length - 1, QR_MODULE_DARK);
	test_eq(qr_module_get(qr, qr->side_length - 1, qr->side_length - 1), QR_MODULE_DARK,
		"Last module dark incorrect");

	qr_module_set(qr, qr->side_length - 1, qr->side_length - 1, QR_MODULE_LIGHT);
	test_eq(qr_module_get(qr, qr->side_length - 1, qr->side_length - 1), QR_MODULE_LIGHT,
		"Last module light incorrect");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test reserved module detection (v1)
 */
TEST(reserved_module_detection_version1)
{
	size_t i, j;
	unsigned reserved = 0;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Test finder patterns (3 corners) and separators
	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			test_eq(qr_module_is_reserved(qr, i, j), 1, "Finder pattern reserved");
			test_eq(qr_module_is_reserved(qr, i, qr->side_length - j + 1), 1, "Finder pattern reserved");
			test_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, j), 1, "Finder pattern reserved");
		}
	}

	// Test timing patterns (horizontal and vertical)
	for (i = 8; i < 13; ++i)
	{
		test_eq(qr_module_is_reserved(qr, 6, i), 1, "Timing pattern reserved");
		test_eq(qr_module_is_reserved(qr, i, 6), 1, "Timing pattern reserved");
	}

	// Test format info areas
	for (i = 0; i < 8; ++i)
	{
		test_eq(qr_module_is_reserved(qr, 8, i), 1, "Format info reserved");
		test_eq(qr_module_is_reserved(qr, i, 8), 1, "Format info reserved");
		test_eq(qr_module_is_reserved(qr, 8, qr->side_length - i + 1), 1, "Format info reserved");
		test_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, 8), 1, "Format info reserved");
	}

	// Version 1 should not have version info reserved
	test_eq(qr_module_is_reserved(qr, 0, qr->side_length - 9), 0, "Version info should not be reserved");

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (qr_module_is_reserved(qr, i, j))
				++reserved;
		}
	}

	test_eq(reserved, 233, "Reserved module count incorrect");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

TEST(reserved_module_detection_version7)
{
	size_t i, j;
	unsigned reserved = 0;

	qr_code *qr = qr_create(7, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			test_eq(qr_module_is_reserved(qr, i, j), 1, "Finder pattern reserved");
			test_eq(qr_module_is_reserved(qr, i, qr->side_length - j + 1), 1, "Finder pattern reserved");
			test_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, j), 1, "Finder pattern reserved");
		}
	}

	for (i = 8; i < 13; ++i)
	{
		test_eq(qr_module_is_reserved(qr, 6, i), 1, "Timing pattern reserved");
		test_eq(qr_module_is_reserved(qr, i, 6), 1, "Timing pattern reserved");
	}

	for (i = 0; i < 8; ++i)
	{
		test_eq(qr_module_is_reserved(qr, 8, i), 1, "Format info reserved");
		test_eq(qr_module_is_reserved(qr, i, 8), 1, "Format info reserved");
		test_eq(qr_module_is_reserved(qr, 8, qr->side_length - i + 1), 1, "Format info reserved");
		test_eq(qr_module_is_reserved(qr, qr->side_length - i + 1, 8), 1, "Format info reserved");
	}

	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 6; ++j)
		{
			test_eq(qr_module_is_reserved(qr, j, qr->side_length - i - 9), 1, "Version info reserved");
			test_eq(qr_module_is_reserved(qr, qr->side_length - i - 9, j), 1, "Version info reserved");
		}
	}

	for (i = 0; i < 5; ++i)
	{
		for (j = 0; j < 5; ++j)
		{
			test_eq(qr_module_is_reserved(qr, 4 + i, 20 + j), 1, "Alignment pattern reserved");
			test_eq(qr_module_is_reserved(qr, 20 + i, 4 + j), 1, "Alignment pattern reserved");
			test_eq(qr_module_is_reserved(qr, 20 + i, 20 + j), 1, "Alignment pattern reserved");
			test_eq(qr_module_is_reserved(qr, 20 + i, 36 + j), 1, "Alignment pattern reserved");
			test_eq(qr_module_is_reserved(qr, 36 + i, 20 + j), 1, "Alignment pattern reserved");
			test_eq(qr_module_is_reserved(qr, 36 + i, 36 + j), 1, "Alignment pattern reserved");
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

	test_eq(reserved, 457, "Reserved module count incorrect");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test codeword placement respects reserved modules
 */
TEST(codeword_placement_reserved)
{
	size_t i, j, v;
	const unsigned versions[] = { 1, 2, 7, 8, 40 }; // Test versions 1, 2, 7, 8, 40

	// Test multiple versions to ensure codeword placement respects reserved modules
	for (v = 0; v < sizeof(versions) / sizeof(versions[0]); ++v)
	{
		qr_code *qr = qr_create(versions[v], QR_MODE_BYTE, QR_EC_LEVEL_H);
		if (!qr) return TEST_FAILURE("Failed to create QR code");

		// Clear matrix and set all codewords to 1 (dark)
		memset(qr->matrix, 0, qr->side_length * qr->side_length * sizeof(qr_module));
		memset(qr->codewords, 0xFF, qr->codeword_count * sizeof(word));
		qr_place_codewords(qr);

		// Verify reserved modules remain light, non-reserved become dark
		for (i = 0; i < qr->side_length; ++i)
		{
			for (j = 0; j < qr->side_length; ++j)
			{
				if (qr_module_is_reserved(qr, i, j))
				{
					test_eq(qr_module_get(qr, i, j), QR_MODULE_LIGHT,
						"Reserved module incorrect");
				}
				else
				{
					// Skip remainder bits in last 12 rows, first 2 columns
					if (i >= qr->side_length - 12 && j < 2) continue;

					test_eq(qr_module_get(qr, i, j), QR_MODULE_DARK,
						"Non-reserved module incorrect");
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
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Fill codewords with sequential values [0, 1, 2, ...]
	for (i = 0; i < qr->codeword_count; ++i)
		qr->codewords[i] = (word) i;

	qr_place_codewords(qr);

	// Test first codeword (0) placement in bottom-right corner
	test_eq(qr_module_get(qr, 20, 20), QR_MODULE_LIGHT, "Codeword 0 bit 8 incorrect");
	test_eq(qr_module_get(qr, 20, 19), QR_MODULE_LIGHT, "Codeword 0 bit 7 incorrect");
	test_eq(qr_module_get(qr, 19, 20), QR_MODULE_LIGHT, "Codeword 0 bit 6 incorrect");
	test_eq(qr_module_get(qr, 19, 19), QR_MODULE_LIGHT, "Codeword 0 bit 5 incorrect");
	test_eq(qr_module_get(qr, 18, 20), QR_MODULE_LIGHT, "Codeword 0 bit 4 incorrect");
	test_eq(qr_module_get(qr, 18, 19), QR_MODULE_LIGHT, "Codeword 0 bit 3 incorrect");
	test_eq(qr_module_get(qr, 17, 20), QR_MODULE_LIGHT, "Codeword 0 bit 2 incorrect");
	test_eq(qr_module_get(qr, 17, 19), QR_MODULE_LIGHT, "Codeword 0 bit 1 incorrect");

	// Test 13th codeword (12) placement - has some dark bits
	test_eq(qr_module_get(qr, 20, 12), QR_MODULE_LIGHT, "Codeword 12 bit 8 incorrect");
	test_eq(qr_module_get(qr, 20, 11), QR_MODULE_LIGHT, "Codeword 12 bit 7 incorrect");
	test_eq(qr_module_get(qr, 19, 12), QR_MODULE_LIGHT, "Codeword 12 bit 6 incorrect");
	test_eq(qr_module_get(qr, 19, 11), QR_MODULE_LIGHT, "Codeword 12 bit 5 incorrect");
	test_eq(qr_module_get(qr, 18, 12), QR_MODULE_DARK, "Codeword 12 bit 4 incorrect");
	test_eq(qr_module_get(qr, 18, 11), QR_MODULE_DARK, "Codeword 12 bit 3 incorrect");
	test_eq(qr_module_get(qr, 17, 11), QR_MODULE_LIGHT, "Codeword 12 bit 2 incorrect");
	test_eq(qr_module_get(qr, 17, 12), QR_MODULE_LIGHT, "Codeword 12 bit 1 incorrect");

	// Test 19th codeword (18) placement in upper area
	test_eq(qr_module_get(qr, 4, 10), QR_MODULE_LIGHT, "Codeword 18 bit 8 incorrect");
	test_eq(qr_module_get(qr, 4, 9), QR_MODULE_LIGHT, "Codeword 18 bit 7 incorrect");
	test_eq(qr_module_get(qr, 5, 10), QR_MODULE_LIGHT, "Codeword 18 bit 6 incorrect");
	test_eq(qr_module_get(qr, 5, 9), QR_MODULE_DARK, "Codeword 18 bit 5 incorrect");
	test_eq(qr_module_get(qr, 7, 10), QR_MODULE_LIGHT, "Codeword 18 bit 4 incorrect");
	test_eq(qr_module_get(qr, 7, 9), QR_MODULE_LIGHT, "Codeword 18 bit 3 incorrect");
	test_eq(qr_module_get(qr, 8, 10), QR_MODULE_DARK, "Codeword 18 bit 2 incorrect");
	test_eq(qr_module_get(qr, 8, 9), QR_MODULE_LIGHT, "Codeword 18 bit 1 incorrect");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

TEST(codeword_placement_wrap_version21)
{
	size_t i;

	qr_code *qr = qr_create(21, QR_MODE_BYTE, QR_EC_LEVEL_H);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Fill codewords with sequential values [0, 1, 2, ...]
	for (i = 0; i < qr->codeword_count; ++i)
		qr->codewords[i] = (word) i;

	qr_place_codewords(qr);

	// Test 56th codeword (55) placement in version 21
	test_eq(qr_module_get(qr, 54, 96), QR_MODULE_LIGHT, "Codeword 55 bit 8 incorrect");
	test_eq(qr_module_get(qr, 54, 95), QR_MODULE_LIGHT, "Codeword 55 bit 7 incorrect");
	test_eq(qr_module_get(qr, 53, 96), QR_MODULE_DARK, "Codeword 55 bit 6 incorrect");
	test_eq(qr_module_get(qr, 53, 95), QR_MODULE_DARK, "Codeword 55 bit 5 incorrect");
	test_eq(qr_module_get(qr, 47, 96), QR_MODULE_LIGHT, "Codeword 55 bit 4 incorrect");
	test_eq(qr_module_get(qr, 47, 95), QR_MODULE_DARK, "Codeword 55 bit 3 incorrect");
	test_eq(qr_module_get(qr, 46, 96), QR_MODULE_DARK, "Codeword 55 bit 2 incorrect");
	test_eq(qr_module_get(qr, 46, 95), QR_MODULE_DARK, "Codeword 55 bit 1 incorrect");

	// Test 104th codeword (103) placement
	test_eq(qr_module_get(qr, 0, 89), QR_MODULE_LIGHT, "Codeword 103 bit 8 incorrect");
	test_eq(qr_module_get(qr, 1, 89), QR_MODULE_DARK, "Codeword 103 bit 7 incorrect");
	test_eq(qr_module_get(qr, 2, 89), QR_MODULE_DARK, "Codeword 103 bit 6 incorrect");
	test_eq(qr_module_get(qr, 3, 89), QR_MODULE_LIGHT, "Codeword 103 bit 5 incorrect");
	test_eq(qr_module_get(qr, 4, 89), QR_MODULE_LIGHT, "Codeword 103 bit 4 incorrect");
	test_eq(qr_module_get(qr, 5, 89), QR_MODULE_DARK, "Codeword 103 bit 3 incorrect");
	test_eq(qr_module_get(qr, 7, 89), QR_MODULE_DARK, "Codeword 103 bit 2 incorrect");
	test_eq(qr_module_get(qr, 7, 90), QR_MODULE_DARK, "Codeword 103 bit 1 incorrect");

	// Test 200th codeword (199) placement
	test_eq(qr_module_get(qr, 13, 84), QR_MODULE_DARK, "Codeword 199 bit 8 incorrect");
	test_eq(qr_module_get(qr, 13, 83), QR_MODULE_DARK, "Codeword 199 bit 7 incorrect");
	test_eq(qr_module_get(qr, 12, 84), QR_MODULE_LIGHT, "Codeword 199 bit 6 incorrect");
	test_eq(qr_module_get(qr, 12, 83), QR_MODULE_LIGHT, "Codeword 199 bit 5 incorrect");
	test_eq(qr_module_get(qr, 11, 84), QR_MODULE_LIGHT, "Codeword 199 bit 4 incorrect");
	test_eq(qr_module_get(qr, 11, 83), QR_MODULE_DARK, "Codeword 199 bit 3 incorrect");
	test_eq(qr_module_get(qr, 10, 84), QR_MODULE_DARK, "Codeword 199 bit 2 incorrect");
	test_eq(qr_module_get(qr, 10, 83), QR_MODULE_DARK, "Codeword 199 bit 1 incorrect");

	qr_destroy(qr);
	return TEST_SUCCESS;
}
