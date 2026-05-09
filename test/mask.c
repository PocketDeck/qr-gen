/**
 * @file mask.c
 * @brief Test cases for QR code masking functionality
 */

#include <qr/mask.h>
#include <qr/matrix.h>
#include <qr/patterns.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <test/base.h>

// Include the source file directly to test static/internal functions
#include <qr/mask.c>

/**
 * @brief Initializes a QR code matrix with random values
 */
static void
qr_init_random(qr_code *qr, unsigned int seed)
{
	size_t i, j;

	srand(seed);

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (!qr_module_is_reserved(qr, i, j))
			{
				qr_module_set(qr, i, j, rand() & 1);
			}
		}
	}

	qr_finder_patterns_apply(qr);
	qr_separators_apply(qr);
	qr_alignment_patterns_apply(qr);
	qr_timing_patterns_apply(qr);
	qr_format_info_apply(qr);
	qr_version_info_apply(qr);
}

/**
 * @brief Test mask pattern selection with random matrices
 */
TEST(mask_selection_optimality)
{
	const int num_tests = 5;
	int test_case, pattern;

	for (test_case = 0; test_case < num_tests; ++test_case)
	{
		int best_score = INT_MAX, score;
		int pattern_scores[QR_MASK_PATTERN_COUNT] = { 0 };

		qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
		if (!qr) return TEST_FAILURE("Failed to create test QR code");

		qr_init_random(qr, (unsigned int) test_case);

		for (pattern = 0; pattern < QR_MASK_PATTERN_COUNT; ++pattern)
		{
			qr_mask_apply_pattern(qr, pattern);
			score = qr_mask_evaluate(qr);
			pattern_scores[pattern] = score;
			qr_mask_apply_pattern(qr, pattern);

			if (score < best_score) best_score = score;
		}

		for (pattern = 0; pattern < QR_MASK_PATTERN_COUNT; ++pattern)
		{
			test_expect_ge(pattern_scores[pattern], best_score,
				"Selected pattern should have the lowest penalty score");
		}
	}

	return TEST_SUCCESS;
}

/**
 * @brief Test Feature 1: 5+ consecutive modules (Penalty = 3 + (N-5))
 */
TEST(mask_feature_1)
{
	size_t i, j;
	int score;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Set matrix to checkerboard
	for (i = 0; i < qr->side_length; ++i)
		for (j = 0; j < qr->side_length; ++j)
			qr_module_set(qr, i, j, (i + j) % 2 ? QR_MODULE_LIGHT : QR_MODULE_DARK);

	// Row 10: 5 dark modules -> score 3
	for (i = 0; i < 5; ++i) qr_module_set(qr, 10, i, QR_MODULE_DARK);
	score = feature_1_evaluation(qr);
	test_expect_eq(score, 3, "Feature 1: 5 consecutive dark modules should score 3");

	// Row 10: 7 dark modules -> score 5
	qr_module_set(qr, 10, 5, QR_MODULE_DARK);
	qr_module_set(qr, 10, 6, QR_MODULE_DARK);
	score = feature_1_evaluation(qr);
	test_expect_eq(score, 5, "Feature 1: 7 consecutive dark modules should score 5");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test Feature 2: 2x2 blocks (Penalty = 3 * (M-1)*(N-1))
 */
TEST(mask_feature_2)
{
	size_t i, j;
	int score;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Set matrix to checkerboard
	for (i = 0; i < qr->side_length; ++i)
		for (j = 0; j < qr->side_length; ++j)
			qr_module_set(qr, i, j, (i + j) % 2 ? QR_MODULE_LIGHT : QR_MODULE_DARK);

	// One 2x2 block at (10,10) -> score 3
	qr_module_set(qr, 10, 10, QR_MODULE_DARK);
	qr_module_set(qr, 10, 11, QR_MODULE_DARK);
	qr_module_set(qr, 11, 10, QR_MODULE_DARK);
	qr_module_set(qr, 11, 11, QR_MODULE_DARK);
	score = feature_2_evaluation(qr);
	test_expect_eq(score, 3, "Feature 2: One 2x2 block should score 3");

	// Overlapping 2x2 blocks: 2x3 block -> two 2x2 blocks -> score 6
	qr_module_set(qr, 10, 12, QR_MODULE_DARK);
	qr_module_set(qr, 11, 12, QR_MODULE_DARK);
	score = feature_2_evaluation(qr);
	test_expect_eq(score, 6, "Feature 2: Two overlapping 2x2 blocks (2x3) should score 6");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test Feature 3: Pattern 1011101 + 4 modules of quiet zone (Penalty = 40)
 */
TEST(mask_feature_3)
{
	size_t i, j;
	int score;
	const qr_module_state pattern[] =
	{
		QR_MODULE_DARK,
		QR_MODULE_LIGHT,
		QR_MODULE_DARK,
		QR_MODULE_DARK,
		QR_MODULE_DARK,
		QR_MODULE_LIGHT,
		QR_MODULE_DARK
	};

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Set matrix all white
	for (i = 0; i < qr->side_length; ++i)
		for (j = 0; j < qr->side_length; ++j)
			qr_module_set(qr, i, j, QR_MODULE_LIGHT);

	// Dark-Light-Dark-Dark-Dark-Light-Dark (1011101)
	// ZXing/Standard: Pattern 1:1:3:1:1 plus 4 modules of LIGHT on either side
	for (j = 4; j < 11; ++j) qr_module_set(qr, 10, j, pattern[j - 4] ? QR_MODULE_DARK : QR_MODULE_LIGHT);
	score = feature_3_evaluation(qr);
	test_expect_eq(score, 40, "Feature 3: Pattern 00001011101 should score 40");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Test Feature 4: Dark module ratio (Penalty = 10 * floor(abs(dark_pct - 50)/5))
 */
TEST(mask_feature_4)
{
	size_t i, j;
	int score;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	// Total modules in V1 = 21*21 = 441
	// 50% = 220.5 modules. 
	// Set 245 modules to DARK (approx 55.5%) -> abs(55.5-50) = 5.5 -> score 10
	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (i * qr->side_length + j < 245)
				qr_module_set(qr, i, j, QR_MODULE_DARK);
			else
				qr_module_set(qr, i, j, QR_MODULE_LIGHT);
		}
	}
	score = feature_4_evaluation(qr);
	test_expect_eq(score, 10, "Feature 4: 55% dark modules should score 10 penalty points");

	qr_destroy(qr);
	return TEST_SUCCESS;
}

/**
 * @brief Tests mask pattern application and toggling behavior
 */
TEST(mask_patterns)
{
	size_t i, j;
	int pattern;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (!qr_module_is_reserved(qr, i, j))
			{
				qr_module_set(qr, i, j, ((i * 3 + j * 5) % 7) < 4 ? QR_MODULE_DARK : QR_MODULE_LIGHT);
			}
			else
			{
				qr_module_set(qr, i, j, QR_MODULE_LIGHT);
			}
		}
	}

	qr_module_state *original = test_malloc(qr->side_length * qr->side_length * sizeof(qr_module_state));
	if (!original) return TEST_FAILURE("Original matrix allocation failed");
	memcpy(original, qr->matrix, qr->side_length * qr->side_length * sizeof(qr_module_state));

	for (pattern = 0; pattern < QR_MASK_PATTERN_COUNT; ++pattern)
	{
		qr_mask_apply_pattern(qr, pattern);

		for (i = 0; i < qr->side_length; ++i)
		{
			for (j = 0; j < qr->side_length; ++j)
			{
				int should_toggle = 0;
				qr_module_state expected;

				if (qr_module_is_reserved(qr, i, j))
				{
					test_expect_eq(qr_module_get(qr, i, j), original[i * qr->side_length + j],
						"Reserved module was modified by mask pattern");
					continue;
				}

				switch (pattern)
				{
					case 0: should_toggle = ((i + j) % 2 == 0); break;
					case 1: should_toggle = (i % 2 == 0); break;
					case 2: should_toggle = (j % 3 == 0); break;
					case 3: should_toggle = ((i + j) % 3 == 0); break;
					case 4: should_toggle = (((i / 2) + (j / 3)) % 2 == 0); break;
					case 5: should_toggle = (((i * j) % 2) + ((i * j) % 3)) == 0; break;
					case 6: should_toggle = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0); break;
					case 7: should_toggle = ((((i + j) % 2) + ((i * j) % 3)) % 2 == 0); break;
				}

				expected = original[i * qr->side_length + j] ^ should_toggle;
				test_expect_eq(qr_module_get(qr, i, j), expected,
					"Mask pattern should toggle modules according to formula");
			}
		}

		qr_mask_apply_pattern(qr, pattern);

		for (i = 0; i < qr->side_length; ++i)
		{
			for (j = 0; j < qr->side_length; ++j)
			{
				test_expect_eq(qr_module_get(qr, i, j), original[i * qr->side_length + j],
					"Double mask application didn't return to original");
			}
		}
	}

	qr_destroy(qr);

	return TEST_SUCCESS;
}

typedef struct {
	const char *name;
	const char *pattern;
	unsigned version;
	qr_ec_level level;
	int expected_scores[QR_MASK_PATTERN_COUNT];
} mask_penalty_test_case;

/**
 * @brief Helper function to initialize a QR code from a pattern string
 */
static void
qr_init_from_pattern(qr_code *qr, const mask_penalty_test_case *test)
{
	size_t i, j;
	const char *p = test->pattern;

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			qr_module_set(qr, i, j, *p == ' ' ? QR_MODULE_LIGHT : QR_MODULE_DARK);
			++p;
		}

		while (*p == '\n' || *p == '\r') ++p;
	}
}

/**
 * @brief Test mask penalty calculation for specific patterns
 */
TEST(mask_penalty_calculation)
{
	const mask_penalty_test_case tests[] =
	{
		{
			.name = "Standard Example V1",
			.pattern =
				"XXXXXXX  XXX  XXXXXXX\n"
				"X     X   X   X     X\n"
				"X XXX X  XXX  X XXX X\n"
				"X XXX X       X XXX X\n"
				"X XXX X       X XXX X\n"
				"X     X  X XX X     X\n"
				"XXXXXXX X X X XXXXXXX\n"
				"            X        \n"
				"      X  X XX        \n"
				"  X XX XXXXX XX XX  X\n"
				"X X X XX XX  XXX X X \n"
				"XX  X  XXXXX X XXX   \n"
				"XXX   X      XXX  X X\n"
				"        XXX    XXXX X\n"
				"XXXXXXX  X XX XX   X \n"
				"X     X  X  XX   X XX\n"
				"X XXX X  XXXX X   X X\n"
				"X XXX X   X  XX XXX  \n"
				"X XXX X  X           \n"
				"X     X  XX  X XXX   \n"
				"XXXXXXX    X  XXX XX ",
			.version = 1,
			.level = QR_EC_LEVEL_L,
			.expected_scores = { 342, 632, 315, 326, 496, 411, 577, 341 }
		},
		{
			.name = "HELLO, world! 123",
			.pattern =
				"XXXXXXX  XXX X    XXXXXXX\n"
				"X     X    XXXX X X     X\n"
				"X XXX X  XXXXX X  X XXX X\n"
				"X XXX X    X  X X X XXX X\n"
				"X XXX X  XX    X  X XXX X\n"
				"X     X           X     X\n"
				"XXXXXXX X X X X X XXXXXXX\n"
				"         XXXX XX         \n"
				"      X  X X    X        \n"
				" X     XX XX XXX XXX XX  \n"
				"  XX XXXX XXXXX X  XX  X \n"
				"XX X    X X XX    XXXX X \n"
				"  X XXXX  XXX X XX  X  X \n"
				" X X      XXXX X  XX XX  \n"
				"  XXX X XX  XXX  XXXX  X \n"
				"       XXXX XXX    X X   \n"
				"  XX  X X X  XX XXXXX   X\n"
				"        X  X X  X   XXX  \n"
				"XXXXXXX  XXXX  XX X XXXX \n"
				"X     X  X  X X X   X  X \n"
				"X XXX X  X  XXX XXXXX X  \n"
				"X XXX X   X X XX X XXXXX \n"
				"X XXX X  X   XX   XX     \n"
				"X     X   XXX X XX       \n"
				"XXXXXXX   XXX         XX ",
			.version = 2,
			.level = QR_EC_LEVEL_Q,
			.expected_scores = { 644, 780, 529, 594, 475, 915, 725, 616 }
		},
		{
			.name = "One really long Text. This should REALLY be a lorem ipsum but i don't know it by heart <3",
			.pattern =
				"XXXXXXX  X X X  XXX XX     XX   XX  XX    X   XXXXXXX\n"
				"X     X     X XX   XXXX  XXXXX X XX   XXX XX  X     X\n"
				"X XXX X  X  XXX X   X   X XX      X     X  X  X XXX X\n"
				"X XXX X   XX   XXX    XXXX X XX  XXXX    XX X X XXX X\n"
				"X XXX X     XXX X  XX   XXXXXXXX    XX   XX   X XXX X\n"
				"X     X  X X XXXXXXX XX X   X X X XXX XX  X   X     X\n"
				"XXXXXXX X X X X X X X X X X X X X X X X X X X XXXXXXX\n"
				"         X X X  XXX   XXX   X  X  XXXX  X XX         \n"
				"      X  X  X   X  X   XXXXXX XX  XX X X    X        \n"
				"     X XXX XX  XXXXX XX  XXX   X X  XX   X X X X X  X\n"
				" XXX  XX    XXX  XXXX X     X XX X    XXX X XXX XX X \n"
				" X     X X    XXXX  X  X     X X XX XX          XX   \n"
				"XX  X X X     X  XX  XX X  XXX XXX X   XX  X XX     X\n"
				"XXX XX X   XXXX X XXXXX X X     XXX X   XX     X XX X\n"
				"  X   X X  XXX X X  XXXX   X    X  XX XXX XX    XXXX \n"
				" XX X   X  XXXX XX  X XXX  XXXXX   XX X X X  X  XX   \n"
				"  XX XXXX X  XX X XXX X  XXX   XXX X   X X  XX X XXXX\n"
				"X X X        X     XXX XX X  XXXX   XX   X X   X X   \n"
				"XX  XXX XXX  X      X      X  XXX     X  XX   XX   X \n"
				"   XXX  X    XXX  X   X XXXXX XX   X   XXXX  X     X \n"
				"   XX XXXX   X X   XXXXX X XXXX  X   X X XX XXXX  X X\n"
				"X XXXX X XX X X X  XXXXX XXX XX   XX   XXX X     X  X\n"
				"XX  XXXXXX XX   X  X X X XXX X   X   X X  X   XXXXX  \n"
				"XXXXXX        XX XX XX X  XX X     XXXXXX  X    X    \n"
				"  XXXXXXX XXXX   X X X XXXXXX XX   X   X  X XXXXX XX \n"
				"XX  X   X X  XX   X  XX X   X   X  X X XXX XX   X  XX\n"
				"X  XX X X      XX   X   X X XX XX    XX  XXXX X X  X \n"
				"XX XX   XX  XX XX    XX X   XXX XX  XXX XXX X   X  XX\n"
				" XX XXXXXXXXX XXX XXX XXXXXXXXX XXXX  X   X XXXXXXXXX\n"
				"  XX X XXX X X X X  XX XX  XX XX  XXX  X XX      XXXX\n"
				"X   XXX XX XXX  XXX XX X  XX        X XX     X XXXXX \n"
				"X  XX  XX XX  X  XXXX     X  XXX    XX  XX X XX  X X \n"
				"XX  XXX  XX X  XXXX  X XX X X  X  XX   X X  XXX X XX \n"
				" XX  X XX    X  XX  XXXXXX X XX X X    X X     X XX X\n"
				"X  X XXX XX XX XXX XX   X   X     X   XX  XX X  XXXX \n"
				"XX X X       XXXXX  X X  X    X XXX X  XXX XXX XXX   \n"
				"    X X X  XXX X X X   X XX  XXX XXX X X X  XXX X XXX\n"
				"  X XX XXXXXX  X    X X  X XXX    X      X X X X  X  \n"
				" X XXXX XXX  XXXXX  X  XX   XX  X     X XXX X   XX X \n"
				" XXX X X   X      X     X XX  X    X XX XXX X XXXX  X\n"
				"XXXX XXXXXX XX  X    XX XXX X XX XXX X X XX    XXXX  \n"
				"XXXX     XXXX  XXXXXXXXXXXXXXXXXX X    X X          X\n"
				"XX XXXXX XX X  X    X XXXX   XXXX X   X  XX XX   X X \n"
				" XX    XXX  XXX    XXX XXXX  XX  XXX   X   XX XXXX   \n"
				"   X  XX   XXX  XX  X XXXXXXX   XX X      X XXXXX  X \n"
				"        XXX    X  X     X   X X   X  X X X  X   X   X\n"
				"XXXXXXX  XX XX    XXX   X X X XXX X   X   X X X X XX \n"
				"X     X     X  XX   XX XX   X XX XX XX  X   X   XX   \n"
				"X XXX X  X XXXX X  XX XXXXXXXXX   X  X      XXXXX X  \n"
				"X XXX X    X  XX X X XXX X X  XXX XX X X X   X  X  X \n"
				"X XXX X   XXXXXXX   XX XXXXXX  XXXX X X XXXXX  X   X \n"
				"X     X     X XX XX X XX  X X X X    XX XXX X    X   \n"
				"XXXXXXX   X X  XX XX X  X    XX XXXXX X   XXX X   XX \n",
			.version = 9,
			.level = QR_EC_LEVEL_H,
			.expected_scores = { 1948, 1964, 1720, 1910, 2267, 2012, 1990, 2325 }
		}
	};
	size_t t;
	const size_t num_tests = sizeof(tests) / sizeof(tests[0]);
	unsigned pattern;

	for (t = 0; t < num_tests; ++t)
	{
		qr_code *qr = qr_create(tests[t].version, QR_MODE_BYTE, tests[t].level);
		if (!qr) return TEST_FAILURE("Failed to create test QR code");

		qr_init_from_pattern(qr, &tests[t]);
		qr_version_info_apply(qr);

		for (pattern = 0; pattern < QR_MASK_PATTERN_COUNT; ++pattern)
		{
			qr_mask_apply_pattern(qr, pattern);
			qr->mask = pattern;
			qr_format_info_apply(qr);

			if (tests[t].expected_scores[pattern] != -1)
			{
				test_expect_eq(qr_mask_evaluate(qr), tests[t].expected_scores[pattern],
					"Penalty score mismatch");
			}

			qr_mask_apply_pattern(qr, pattern);
		}

		qr_destroy(qr);
	}

	return TEST_SUCCESS;
}

/**
 * @brief Test mask evaluation features
 */
TEST(mask_evaluation)
{
	size_t i, j;
	int score;

	qr_code *qr = qr_create(1, QR_MODE_BYTE, QR_EC_LEVEL_L);
	if (!qr) return TEST_FAILURE("Failed to create test QR code");

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			qr_module_set(qr, i, j, QR_MODULE_LIGHT);
		}
	}

	for (i = 0; i < 6; ++i)
	{
		qr_module_set(qr, 5, 5 + i, QR_MODULE_DARK);
	}

	score = qr_mask_evaluate(qr);
	test_expect_gt(score, 0, "Should detect consecutive modules in row/column");

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			qr_module_set(qr, i, j, QR_MODULE_LIGHT);
		}
	}

	qr_module_set(qr, 5, 5, QR_MODULE_DARK);
	qr_module_set(qr, 5, 6, QR_MODULE_DARK);
	qr_module_set(qr, 6, 5, QR_MODULE_DARK);
	qr_module_set(qr, 6, 6, QR_MODULE_DARK);

	score = qr_mask_evaluate(qr);
	test_expect_gt(score, 0, "Should detect 2x2 block of same modules");

	return TEST_SUCCESS;
}

/**
 * @brief Test that different mask patterns are selected for different inputs
 */
TEST(mask_pattern_diversity)
{
	const size_t num_tests = 15;
	const unsigned min_unique_patterns = 3;
	unsigned pattern_counts[QR_MASK_PATTERN_COUNT] = { 0 }, unique_patterns = 0;
	size_t test_case;

	for (test_case = 0; test_case < num_tests; ++test_case)
	{
		qr_code *qr = qr_create(6, QR_MODE_BYTE, QR_EC_LEVEL_L);
		if (!qr) return TEST_FAILURE("Failed to create test QR code");

		qr_init_random(qr, (unsigned int) test_case + 1234);
		qr_mask_apply(qr);

		if (pattern_counts[qr->mask] == 0)
			++unique_patterns;
		pattern_counts[qr->mask]++;

		qr_destroy(qr);

		if (unique_patterns >= min_unique_patterns) break;
	}

	test_expect_ge(unique_patterns, min_unique_patterns,
		"Insufficient pattern diversity");

	return TEST_SUCCESS;
}
