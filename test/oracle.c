/**
 * @file oracle.c
 * @brief Oracle integration tests
 */

#include <qr/enc.h>
#include <qr/matrix.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test/base.h>

BEFORE()
{
	if (system("which curl >/dev/null 2>&1"))
		return TEST_FAILURE("curl not found");

	if (system("which magick >/dev/null 2>&1"))
		return TEST_FAILURE("magick not found");

	if (system("which awk >/dev/null 2>&1"))
		return TEST_FAILURE("awk not found");

	if (system("which pnmtopnm >/dev/null 2>&1"))
		return TEST_FAILURE("pnmtopnm not found");

	return TEST_SUCCESS;
}

#define AWK_PBM_NEWLINES "awk '                 \
NR==1 { next }                                  \
NR==2                                           \
{                                               \
    w = $1;                                     \
    h = $2;                                     \
    next;                                       \
}                                               \
{                                               \
    gsub(/[[:space:]]/, \"\");                  \
    bits = bits $0;                             \
}                                               \
END                                             \
{                                               \
    for (i = 1; i <= h; ++i)                    \
        print substr(bits, (i - 1) * w + 1, w); \
}'"

#define AWK_QUIET_ZONE_REMOVE "awk '    \
{                                       \
    rows[NR] = $0;                      \
    if ($0 ~ /1/)                       \
        used_row[NR] = 1;               \
                                        \
    n = split($0, row, \"\");           \
                                        \
    for (i = 1; i <= n; ++i)            \
    {                                   \
        vals[NR,i] = row[i];            \
        if (row[i] != 0)                \
            used_col[i] = 1;            \
    }                                   \
}                                       \
END                                     \
{                                       \
    for (r = 1; r <= NR; ++r)           \
    {                                   \
        if (used_row[r])                \
        {                               \
            top = r;                    \
            break;                      \
        }                               \
    }                                   \
                                        \
    for (r = NR; r >= 1; --r)           \
    {                                   \
        if (used_row[r])                \
        {                               \
            bottom = r;                 \
            break;                      \
        }                               \
    }                                   \
                                        \
    for (c = 1; c <= n; ++c)            \
    {                                   \
        if (used_col[c])                \
        {                               \
            left = c;                   \
            break;                      \
        }                               \
    }                                   \
                                        \
    for (c = n; c >= 1; --c)            \
    {                                   \
        if (used_col[c])                \
        {                               \
            right = c;                  \
            break;                      \
        }                               \
    }                                   \
                                        \
    for (r = top; r <= bottom; ++r)     \
    {                                   \
        for (c = left; c <= right; ++c) \
        {                               \
            printf(\"%%s\", vals[r,c]); \
        }                               \
    }                                   \
}'"

/**
 * @brief Fetch oracle QR matrix from ZXing API
 */
static char *
fetch_oracle_bits(const char *text, qr_ecl level)
{
	char cmd[4096];
	char ecc = (char []) { 'L', 'M', 'Q', 'H' }[level];

		// Build command to fetch QR code from ZXing API and convert to bit string
	snprintf(cmd, sizeof(cmd),
		"curl -s -G \"https://zxing.org/w/chart\" "
		"--data-urlencode \"chl=%s\" "                   // URL encode text content
		"--data \"chld=%c\" "                            // Error correction level
		"--data \"chs=1x1\" "                            // 1x1 pixel size (just data)
		"--data \"cht=qr\" "                             // QR code type
		"--data \"choe=ISO-8859-1\" 2>/dev/null "        // Character encoding
		"| magick - -threshold 50%% pbm:- 2>/dev/null "  // Convert to PBM with threshold
		"| pnmtopnm -plain 2>/dev/null "                 // Convert to plain PNM
		"| " AWK_PBM_NEWLINES                            // Place line breaks
		"| " AWK_QUIET_ZONE_REMOVE                       // Remove quiet zone
		"| tr -d ' \\n'",                                // Remove newlines
		text, ecc);

	FILE *fp = popen(cmd, "r");
	if (!fp) return nullptr;

	char *bits = test_malloc(32768); /* Large enough for version 40 */
	if (!bits)
	{
		pclose(fp);
		return nullptr;
	}

	if (fgets(bits, 32768, fp) == nullptr)
	{
		pclose(fp);
		return nullptr;
	}

	pclose(fp);

	return bits;
}

/**
 * @brief Initialize QR code from bit string
 */
static void
init_qr_from_bits(qr_code *qr, const char *bits)
{
	size_t i, j;

	// Convert bit string to QR matrix modules
	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			qr_module_set(qr, i, j, bits[i * qr->side_length + j] == '1' ? QR_MODULE_DARK : QR_MODULE_LIGHT);
		}
	}
}

/**
 * @brief Generate side-by-side QR code string
 */
static void
qr_side_by_side_to_str(const qr_code *qr1, const qr_code *qr2, char *buf)
{
	size_t i, j;
	char *p = buf;

	// Create top quiet zone (4 rows)
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < qr1->side_length + 8; ++j) p += sprintf(p, "\x1b[7m  \x1b[27m");
		p += sprintf(p, "    ");
		for (j = 0; j < qr2->side_length + 8; ++j) p += sprintf(p, "\x1b[7m  \x1b[27m");
		p += sprintf(p, "\n");
	}

	// Create side-by-side QR codes with ANSI colors
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

	// Create bottom quiet zone (4 rows)
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
 * @brief Create oracle QR code
 */
static qr_code *
qr_create_oracle(const char *text, qr_ecl level, qr_mode mode)
{
	size_t len, side;
	unsigned version;
	char *bits = fetch_oracle_bits(text, level);
	if (!bits) return nullptr;

	// Calculate QR version from matrix size
	len = strlen(bits);
	for (side = 0; side * side < len; ++side);
	if (side * side != len) return nullptr;

	// Calculate QR version from matrix size
	version = ((side - 21) / 4) + 1;
	qr_code *qr = qr_create(version, mode, level);
	if (!qr) return nullptr;

	// Initialize QR matrix from fetched bit pattern
	init_qr_from_bits(qr, bits);
	return qr;
}

/**
 * @brief Compare QR generation with oracle
 */
static struct test_result
compare_with_oracle(const char *text, qr_ecl level)
{
	qr_code *our_qr, *oracle_qr;
	size_t i, j;

	char *msg = test_malloc(1024 * 1024); /* 1MB */
	if (!msg) return TEST_FAILURE("Memory allocation failed");

	// Determine mode and minimum QR version for input text
	qr_mode mode = qr_detect_mode(text);
	unsigned version = qr_min_version(mode, level, strlen(text));
	if (!version) return TEST_FAILURE("Input too large");

	// Create our QR code and encode text
	our_qr = qr_create(version, mode, level);
	if (!our_qr) return TEST_FAILURE("Failed to create QR code");

	qr_encode_text(our_qr, text);

	// Create oracle QR code from ZXing API
	oracle_qr = qr_create_oracle(text, level, mode);
	if (!oracle_qr) return TEST_FAILURE("Failed to create oracle QR");

	// Verify both QR codes have same dimensions
	if (our_qr->side_length != oracle_qr->side_length)
		test_eq(our_qr->side_length, oracle_qr->side_length, "QR size mismatch");

	// Prepare failure message with side-by-side comparison
	memcpy(msg, "QR Code comparison failed: \n", 28);
	qr_side_by_side_to_str(our_qr, oracle_qr, msg + 28);

	// Compare each module between our and oracle QR codes
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

TEST(oracle_full_capacity_L)
{
	return compare_with_oracle("ABCDEFGHIJKLMNOPQRSTUVWXY", QR_EC_LEVEL_L);
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
