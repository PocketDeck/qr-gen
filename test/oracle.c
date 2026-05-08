/**
 * @file oracle.c
 * @brief Integration tests comparing QR generation with an oracle (ZXing)
 */

#include <qr/enc.h>
#include <qr/matrix.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <test/base.h>

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
static char *
fetch_oracle_bits(const char *text, qr_ec_level level)
{
	char cmd[4096];
	char ecc = (char []) { 'L', 'M', 'Q', 'H' }[level];

	/* Command to fetch oracle matrix and convert to bits */
	/* Uses ZXing API with curl's URL encoding to handle spaces and special characters */
	snprintf(cmd, sizeof(cmd),
		"curl -s -G \"https://zxing.org/w/chart\" "
		"--data-urlencode \"chl=%s\" "
		"--data \"chld=%c\" "
		"--data \"chs=1x1\" "
		"--data \"cht=qr\" "
		"--data \"choe=ISO-8859-1\" 2>/dev/null "
		"| magick - -threshold 50%% pbm:- 2>/dev/null | pnmtoplainpnm 2>/dev/null | sed '1,2d' "
		"| tail -n +5 | head -n -4 | cut -c 5- | rev | cut -c 5- | rev | tr -d ' \\n'",
		text, ecc);

	FILE *fp = popen(cmd, "r");
	if (!fp) return NULL;

	char *bits = test_malloc(32768); /* Large enough for version 40 */
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

	return bits;
}

/**
 * @brief Initialize a QR code structure from a bit string
 */
static void
init_qr_from_bits(qr_code *qr, const char *bits)
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
static void
qr_side_by_side_to_str(const qr_code *qr1, const qr_code *qr2, char *buf)
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
 * @brief Create an oracle QR code object from text and EC level
 */
static qr_code *
qr_create_oracle(const char *text, qr_ec_level level)
{
	char *bits;
	size_t len, side;
	unsigned version;
	qr_code *qr;

	bits = fetch_oracle_bits(text, level);
	if (!bits) return NULL;

	len = strlen(bits);
	side = 0;
	while (side * side < len) ++side;
	if (side * side != len) return NULL;

	version = ((side - 21) / 4) + 1;
	qr = qr_create(version, QR_MODE_BYTE, level);
	if (!qr) return NULL;

	init_qr_from_bits(qr, bits);
	return qr;
}

/**
 * @brief Compare our QR generation with oracle
 */
static struct test_result
compare_with_oracle(const char *text, qr_ec_level level)
{
	unsigned version;
	qr_code *our_qr, *oracle_qr;
	size_t i, j;

	char *msg = test_malloc(1024 * 1024); /* 1MB */
	if (!msg) return TEST_FAILURE("Failed to allocate memory for error message");

	/* Create our QR code */
	version = qr_min_version(strlen(text), level);
	if (!version) return TEST_FAILURE("Input too large");

	our_qr = qr_create(version, QR_MODE_BYTE, level);
	if (!our_qr) return TEST_FAILURE("Failed to create QR code");

	qr_encode_message(our_qr, text);

	/* Create oracle QR code object */
	oracle_qr = qr_create_oracle(text, level);
	if (!oracle_qr) return TEST_FAILURE("Failed to fetch/create oracle QR object");

	/* Compare side length */
	if (our_qr->side_length != oracle_qr->side_length)
		test_expect_eq(our_qr->side_length, oracle_qr->side_length, "Size mismatch between our QR and oracle");

	/* Prepare failure message */
	memcpy(msg, "QR Code comparison failed: \n", 28);
	qr_side_by_side_to_str(our_qr, oracle_qr, msg + 28);
	msg[27] = '\0';

	/* Compare matrices */
	for (i = 0; i < our_qr->side_length; ++i)
	{
		for (j = 0; j < our_qr->side_length; ++j)
		{
			if (qr_module_get(our_qr, i, j) != qr_module_get(oracle_qr, i, j))
				return TEST_FAILURE(msg);
		}
	}

	qr_destroy(our_qr);
	qr_destroy(oracle_qr);
	return TEST_SUCCESS;
}

TEST(oracle_simple_L)
{
	return compare_with_oracle("HELLO", QR_EC_LEVEL_L);
}

TEST(oracle_simple_M)
{
	return compare_with_oracle("HELLO", QR_EC_LEVEL_M);
}

TEST(oracle_simple_Q)
{
	return compare_with_oracle("HELLO", QR_EC_LEVEL_Q);
}

TEST(oracle_simple_H)
{
	return compare_with_oracle("HELLO", QR_EC_LEVEL_H);
}

TEST(oracle_url_M)
{
	return compare_with_oracle("https://example.com", QR_EC_LEVEL_M);
}

TEST(oracle_long_M)
{
	return compare_with_oracle("The quick brown fox jumps over the lazy dog", QR_EC_LEVEL_M);
}

TEST(oracle_very_long_L)
{
	return compare_with_oracle(
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
		"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
		"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
		"nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
		"reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.",
		QR_EC_LEVEL_L
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
		QR_EC_LEVEL_H
	);
}
