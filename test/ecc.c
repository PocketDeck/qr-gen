/**
 * @file ecc.c
 * @brief ECC functionality tests
 */

#include <qr/ecc.h>
#include <qr/qr.h>
#include <qr/tables.h>
#include <qr/types.h>
#include <stddef.h>
#include <test/base.h>

#include <qr/ecc.c>

/**
 * @brief Initialize GF log/antilog tables
 */
BEFORE()
{
	gf_init_log_antilog();
	return TEST_SUCCESS;
}

/**
 * @brief Test GF(2^8) arithmetic operations
 */
TEST(gf_arithmetic)
{
	test_eq(gf_mul(2, 3), 6, "GF multiplication 2*3");
	test_eq(gf_mul(0, 5), 0, "GF multiplication with zero");
	test_eq(gf_mul(7, 1), 7, "GF multiplication with one");

	test_eq(gf_add(5, 3), 6, "GF addition 5+3");
	test_eq(gf_add(0, 4), 4, "GF addition with zero");

	test_eq(gf_mul(0x03, 0x0E), 18, "GF multiplication 0x03*0x0E");
	test_eq(gf_mul(0x1A, 0x0B), 254, "GF multiplication 0x1A*0x0B");

	return TEST_SUCCESS;
}

/**
 * @brief Test generator polynomial creation
 */
TEST(generator_polynomial)
{
	size_t i;
	word poly[30];

	// Test degree 5 generator polynomial: g(x) = (x-a^0)(x-a^1)(x-a^2)(x-a^3)(x-a^4)
	generator_polynomial(poly, 5);

	// Expected coefficients correspond to antilog table values
	word expected5_exponents[5] = { 113, 164, 166, 119, 10 };

	for (i = 0; i < 5; ++i)
	{
		test_eq(poly[i], gf_antilog[expected5_exponents[i]],
			"Degree 5 generator polynomial coefficient");
	}

	// Test degree 16 generator polynomial (used in version 1-M QR codes)
	generator_polynomial(poly, 16);

	word expected16_exponents[16] =
	{
		120, 104, 107, 109, 102, 161, 76, 3, 91,
		191, 147, 169, 182, 194, 225, 120
	};

	for (i = 0; i < 16; ++i)
	{
		test_eq(poly[i], gf_antilog[expected16_exponents[i]],
			"Degree 16 generator polynomial coefficient");
	}

	return TEST_SUCCESS;
}

/**
 * @brief Test ECC generation
 */
TEST(ecc_generation)
{
	size_t i;

	// Test data for Version 1-L QR code (7 data codewords, 10 ECC codewords)
	word data[7] = { 40, 88, 12, 6, 46, 77, 36 };
	word ecc[10] = {};
	word g[10] = {};

	// Generate Reed-Solomon error correction codewords
	generator_polynomial(g, 10);
	ecc_generate(data, 7, ecc, 10, g);

	// Expected ECC values computed from standard Reed-Solomon algorithm
	word expected_ecc[10] = { 214, 246, 18, 193, 38, 69, 160, 197, 199, 15 };
	for (i = 0; i < 10; ++i)
	{
		test_eq(ecc[i], expected_ecc[i],
			"ECC generation failed");
	}

	return TEST_SUCCESS;
}

/**
 * @brief Test single block ECC encoding
 */
TEST(qr_ec_encode_version1_l)
{
	size_t i;
	const size_t data_count = TOTAL_DATA_CODEWORD_COUNT[QR_EC_LEVEL_L][0];
	const size_t total_cw = CODEWORD_COUNT[0];
	const size_t ecc_length = total_cw - data_count;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Fill data codewords with test pattern
	for (i = 0; i < data_count; ++i)
		qr->codewords[i] = (word) ((i * 5 + 7) % 256);

	test_eq(ecc_length, 7, "ECC length incorrect");
	word expected_ecc[7] = { 79, 91, 164, 37, 5, 243, 57 };

	// Generate ECC and append to data codewords
	qr_ec_encode(qr);

	for (i = 0; i < ecc_length; ++i)
	{
		test_eq(qr->codewords[data_count + i], expected_ecc[i],
			"ECC bytes incorrect");
	}

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test multi-block ECC encoding
 */
TEST(qr_ec_encode_version9_m)
{
	size_t i;
	const size_t data_count = TOTAL_DATA_CODEWORD_COUNT[QR_EC_LEVEL_M][8];
	const size_t total_cw = CODEWORD_COUNT[8];
	const size_t ecc_length = total_cw - data_count;

	qr_code *qr = qr_create(9, QR_MODE_BYTE, QR_EC_LEVEL_M);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Fill data codewords with test pattern for multi-block scenario
	for (i = 0; i < data_count; ++i)
		qr->codewords[i] = (word) ((i * 3 + 11) % 256);

	test_eq(ecc_length, 110, "ECC length incorrect");
	word expected_ecc[] = {
		245, 121, 89, 42, 56, 51, 80, 31, 34, 6,
		243, 58, 171, 209, 46, 130, 106, 40, 112, 46,
		180, 40, 74, 135, 42, 23, 111, 54, 210, 161,
		153, 136, 202, 233, 129, 243, 46, 21, 95, 45,
		68, 133, 197, 103, 71, 14, 63, 58, 221, 225,
		230, 210, 30, 138, 19, 133, 47, 135, 26, 148,
		91, 129, 144, 112, 183, 173, 177, 150, 199, 45,
		176, 197, 180, 45, 54, 150, 141, 83, 222, 236,
		208, 167, 126, 21, 92, 102, 211, 243, 16, 237,
		26, 135, 99, 215, 36, 93, 95, 254, 118, 59,
		166, 41, 207, 255, 12, 246, 195, 228, 204, 153,
	};

	// Generate ECC for multi-block QR code (version 9-M)
	qr_ec_encode(qr);

	for (i = 0; i < ecc_length; ++i)
	{
		test_eq(qr->codewords[data_count + i], expected_ecc[i],
			"ECC bytes incorrect");
	}

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test codeword interleaving (single block)
 */
TEST(codeword_interleaving_version1_h)
{
	size_t i;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_H);

	// Fill codewords with sequential values [1, 2, 3, ...]
	for (i = 0; i < qr->codeword_count; ++i)
		qr->codewords[i] = (word) (i + 1);

	// Single block: interleaving should not change order
	qr_interleave_codewords(qr);

	for (size_t i = 0; i < qr->codeword_count; ++i)
	{
		test_eq(qr->codewords[i], i + 1,
			"Single block codeword order incorrect");
	}

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test codeword interleaving (multi-block)
 */
TEST(codeword_interleaving_version8_m)
{
	size_t i;

	qr_code *qr = qr_create(8, QR_MODE_BYTE, QR_EC_LEVEL_M);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	// Version 8-M has 4 blocks: 2x(38 data + 22 ECC) + 2x(39 data + 22 ECC)
	// Fill with sequential values to test interleaving across blocks
	for (i = 0; i < 242; ++i)
		qr->codewords[i] = (word) (i + 1);

	// Interleave codewords: take first from each block, then second, etc.
	qr_interleave_codewords(qr);

	// Test first few interleaved data codewords
	test_eq(qr->codewords[0], 1, "Data codeword incorrect");
	test_eq(qr->codewords[1], 39, "Data codeword incorrect");
	test_eq(qr->codewords[2], 77, "Data codeword incorrect");

	test_eq(qr->codewords[3], 116, "Data codeword incorrect");
	test_eq(qr->codewords[4], 2, "Data codeword incorrect");
	test_eq(qr->codewords[5], 40, "Data codeword incorrect");

	// Test last few interleaved data codewords
	test_eq(qr->codewords[148], 38, "Data codeword incorrect");
	test_eq(qr->codewords[149], 76, "Data codeword incorrect");
	test_eq(qr->codewords[152], 115, "Data codeword incorrect");
	test_eq(qr->codewords[153], 154, "Data codeword incorrect");

	// Test first few interleaved ECC codewords
	test_eq(qr->codewords[154], 155, "ECC codeword incorrect");
	test_eq(qr->codewords[155], 177, "ECC codeword incorrect");
	test_eq(qr->codewords[156], 199, "ECC codeword incorrect");
	test_eq(qr->codewords[157], 221, "ECC codeword incorrect");
	test_eq(qr->codewords[158], 156, "ECC codeword incorrect");
	test_eq(qr->codewords[159], 178, "ECC codeword incorrect");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test ECC table consistency
 */
TEST(ecc_table_consistency)
{
	qr_ecl level;
	size_t version;
	size_t total_codewords, total_data_codewords;
	size_t block_type;

	// Verify ECC table consistency across all versions and error correction levels
	for (level = 0; level < QR_EC_LEVEL_COUNT; ++level)
	{
		for (version = 0; version < QR_VERSION_COUNT; ++version)
		{
			total_codewords = 0;
			total_data_codewords = 0;

			// Check each block type for consistency
			for (block_type = 0; block_type < BLOCK_TYPES_PER_VERSION; ++block_type)
			{
				size_t block_count = BLOCK_COUNT[level][version][block_type];
				size_t total_cw = TOTAL_CODEWORD_COUNT[level][version][block_type];
				size_t data_cw = DATA_CODEWORD_COUNT[level][version][block_type];

				// Empty blocks should have zero counts
				if (block_count == 0)
				{
					test_eq(total_cw, 0, "Empty block has non-zero total");
					test_eq(data_cw, 0, "Empty block has non-zero data");
					continue;
				}

				// Total codewords must be >= data codewords
				test_ge(total_cw, data_cw,
					"Total < data codewords");

				total_codewords += block_count * total_cw;
				total_data_codewords += block_count * data_cw;
			}

			// Verify computed totals match precomputed values
			test_eq(total_data_codewords, TOTAL_DATA_CODEWORD_COUNT[level][version],
				"Total data codewords mismatch");

			test_eq(total_codewords, CODEWORD_COUNT[version],
				"Total codewords mismatch");
		}
	}

	return TEST_SUCCESS;
}
