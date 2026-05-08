#include <qr/ecc.h>
#include <qr/enc.h>
#include <qr/info.h>
#include <qr/mask.h>
#include <qr/matrix.h>
#include <qr/patterns.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test/base.h>

/**
 * @file oracle.c
 * @brief Integration tests comparing QR generation with an oracle (ZXing)
 */

BEFORE()
{
	if (system("which curl >/dev/null 2>&1"))
		return TEST_FAILURE("Could not find curl, skipping suite...");
	if (system("which magick >/dev/null 2>&1"))
		return TEST_FAILURE("Could not find magick, skipping suite...");
	return TEST_SUCCESS;
}

/**
 * @brief Fetch oracle QR matrix from ZXing API
 *
 * Uses curl to download and ImageMagick to process into a bit string.
 */
static char *fetch_oracle_bits(const char *text, const char *ecc)
{
	char cmd[4096];
	char *bits;
	FILE *fp;
	size_t size;

	/* Command to fetch oracle matrix and convert to bits */
	/* Uses ZXing API with curl's URL encoding to handle spaces and special characters */
	snprintf(cmd, sizeof(cmd),
		"curl -s -G \"https://zxing.org/w/chart\" "
		"--data-urlencode \"chl=%s\" "
		"--data \"chld=%s\" "
		"--data \"chs=1x1\" "
		"--data \"cht=qr\" "
		"--data \"choe=ISO-8859-1\" 2>/dev/null "
		"| magick - -threshold 50%% pbm:- 2>/dev/null | pnmtoplainpnm 2>/dev/null | sed '1,2d' "
		"| tail -n +5 | head -n -4 | cut -c 5- | rev | cut -c 5- | rev | tr -d ' \\n'",
		text, ecc);

	fp = popen(cmd, "r");
	if (!fp) return NULL;

	bits = test_malloc(32768); /* Large enough for version 40 */
	if (!bits)
	{
		pclose(fp);
		return NULL;
	}

	if (fgets(bits, 32768, fp) == NULL)
	{
		pclose(fp);
		return NULL;
	}

	pclose(fp);

	/* Remove potential newline */
	size = strlen(bits);
	if (size > 0 && bits[size - 1] == '\n')
		bits[size - 1] = '\0';

	return bits;
}

/**
 * @brief Initialize a QR code structure from a bit string
 */
static void init_qr_from_bits(qr_code *qr, const char *bits)
{
	size_t i, j;

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			qr_module_set(qr, i, j, bits[i * qr->side_length + j] == '1' ? QR_MODULE_DARK : QR_MODULE_LIGHT);
		}
	}
}

/**
 * @brief Generate a string containing two QR codes side-by-side with a 2-black-space gap
 */
static void qr_side_by_side_to_str(const qr_code *qr1, const qr_code *qr2, char *buf)
{
	size_t i, j;
	char *p = buf;

	/* Top quiet zone */
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < qr1->side_length + 8; ++j) p += sprintf(p, "\x1b[7m  \x1b[27m");
		p += sprintf(p, "    ");
		for (j = 0; j < qr2->side_length + 8; ++j) p += sprintf(p, "\x1b[7m  \x1b[27m");
		p += sprintf(p, "\n");
	}

	for (i = 0; i < qr1->side_length; ++i)
	{
		/* qr1: left quiet zone */
		p += sprintf(p, "\x1b[7m        \x1b[27m");
		for (j = 0; j < qr1->side_length; ++j) p += sprintf(p, "%s", qr_module_get(qr1, i, j) ? "  " : "\x1b[7m  \x1b[27m");
		/* qr1: right quiet zone */
		p += sprintf(p, "\x1b[7m        \x1b[27m");

		p += sprintf(p, "    ");

		/* qr2: left quiet zone */
		p += sprintf(p, "\x1b[7m        \x1b[27m");
		for (j = 0; j < qr2->side_length; ++j) p += sprintf(p, "%s", qr_module_get(qr2, i, j) ? "  " : "\x1b[7m  \x1b[27m");
		/* qr2: right quiet zone */
		p += sprintf(p, "\x1b[7m        \x1b[27m");

		p += sprintf(p, "\n");
	}

	/* Bottom quiet zone */
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < qr1->side_length + 8; ++j) p += sprintf(p, "\x1b[7m  \x1b[27m");
		p += sprintf(p, "    ");
		for (j = 0; j < qr2->side_length + 8; ++j) p += sprintf(p, "\x1b[7m  \x1b[27m");
		p += sprintf(p, "\n");
	}
}

/**
 * @brief Compare our QR generation with oracle
 */
static struct test_result compare_with_oracle(const char *text, const char *ecc_str)
{
	qr_ec_level level;
	unsigned version;
	qr_code *qr;
	qr_code *oracle_qr;
	char *oracle_bits;
	size_t oracle_len;
	size_t oracle_side;
	size_t i, j;

	char *msg = test_malloc(1024 * 1024); /* 1MB */
	if (!msg) return TEST_FAILURE("Failed to allocate memory for error message");

	/* Parse EC level */
	switch (ecc_str[0])
	{
		case 'L': level = QR_EC_LEVEL_L; break;
		case 'M': level = QR_EC_LEVEL_M; break;
		case 'Q': level = QR_EC_LEVEL_Q; break;
		case 'H': level = QR_EC_LEVEL_H; break;
		default: return TEST_FAILURE("Invalid ECC level");
	}

	/* Create our QR code */
	version = qr_min_version(strlen(text), level);
	if (version >= QR_VERSION_COUNT)
		return TEST_FAILURE("Input too large");

	qr = qr_create(version, QR_MODE_BYTE, level);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	/* Generate QR matrix */
	qr_encode_data(qr, text);
	qr_ec_encode(qr);
	qr_interleave_codewords(qr);
	qr_place_codewords(qr);
	qr_finder_patterns_apply(qr);
	qr_separators_apply(qr);
	qr_timing_patterns_apply(qr);
	qr_alignment_patterns_apply(qr);
	qr_mask_apply(qr);
	qr_format_info_apply(qr);
	qr_version_info_apply(qr);

	/* Fetch oracle bits */
	oracle_bits = fetch_oracle_bits(text, ecc_str);
	if (!oracle_bits) return TEST_FAILURE("Failed to fetch oracle bits");

	oracle_len = strlen(oracle_bits);
	oracle_side = 0;
	while (oracle_side * oracle_side < oracle_len) ++oracle_side;

	if (oracle_side * oracle_side != oracle_len)
		return TEST_FAILURE("Oracle bit string length is not a perfect square");

	/* Oracle from ZXing with chs=1x1 and chld=ECC already strips quiet zone.
	 * Our qr->side_length also doesn't include quiet zone.
	 * So we compare directly. */
	if (qr->side_length != oracle_side)
		test_expect_eq(qr->side_length, oracle_side, "Size mismatch between our QR and oracle");

	/* Create oracle QR code object for side-by-side print if needed */
	oracle_qr = qr_create(qr->version, qr->mode, qr->level);
	if (!oracle_qr)
		return TEST_FAILURE("Failed to create oracle QR object");
	init_qr_from_bits(oracle_qr, oracle_bits);

	memcpy(msg, "QR Code comparison failed: \n", 28);
	qr_side_by_side_to_str(qr, oracle_qr, msg + 28);

	/* Compare matrices */
	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			if (qr_module_get(qr, i, j) != qr_module_get(oracle_qr, i, j))
				return TEST_FAILURE(msg);
		}
	}

	return TEST_SUCCESS;
}

TEST(oracle_simple_L)
{
	return compare_with_oracle("HELLO", "L");
}

TEST(oracle_simple_M)
{
	return compare_with_oracle("HELLO", "M");
}

TEST(oracle_simple_Q)
{
	return compare_with_oracle("HELLO", "Q");
}

TEST(oracle_simple_H)
{
	return compare_with_oracle("HELLO", "H");
}

TEST(oracle_url_M)
{
	return compare_with_oracle("https://example.com", "M");
}

TEST(oracle_long_M)
{
	return compare_with_oracle("The quick brown fox jumps over the lazy dog", "M");
}

TEST(oracle_very_long_L)
{
	return compare_with_oracle(
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
		"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
		"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
		"nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
		"reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.",
		"L"
	);
}

TEST(oracle_very_long_H)
{
	return compare_with_oracle(
		"QR Code (abbreviated from Quick Response Code) is the trademark for a "
		"type of matrix barcode (or two-dimensional barcode) first designed in "
		"1994 for the automotive industry in Japan. A barcode is a "
		"machine-readable optical label that contains information about the "
		"item to which it is attached.",
		"H"
	);
}
