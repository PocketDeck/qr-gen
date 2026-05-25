#include <assert.h>
#include <qr/enc.h>
#include <qr/tables.h>
#include <qr/types.h>
#include <stddef.h>
#include <string.h>

unsigned
qr_min_version(qr_mode mode, qr_ecl level, size_t bytes)
{
	size_t i;

	for (i = 0; i < QR_VERSION_COUNT && bytes > CAPACITY[mode][level][i]; ++i);

	return i == QR_VERSION_COUNT ? 0 : (unsigned) (i + 1);
}

constexpr unsigned INVALID_VALUE = 0x100;

static unsigned
numeric_value(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	return INVALID_VALUE;
}

static unsigned
alphanumeric_value(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
	if (c == ' ') return 36;
	if (c == '$') return 37;
	if (c == '%') return 38;
	if (c == '*') return 39;
	if (c == '+') return 40;
	if (c == '-') return 41;
	if (c == '.') return 42;
	if (c == '/') return 43;
	if (c == ':') return 44;
	return INVALID_VALUE;
}

qr_mode
qr_detect_mode(const char *text)
{
	qr_mode best = QR_MODE_NUMERIC;

	for (; *text; ++text)
	{
		if (numeric_value(*text) == INVALID_VALUE)
			best = QR_MODE_ALPHANUMERIC;

		if (alphanumeric_value(*text) == INVALID_VALUE)
			return QR_MODE_BYTE;
	}

	return best;
}

static void
append_bit(word *buffer, size_t *byte, size_t *bit, bool value)
{
	if (*bit == 0) buffer[*byte] = 0;
	buffer[*byte] |= value << (7 - *bit);

	if (++*bit == 8)
	{
		*bit = 0;
		++*byte;
	}
}

static void
append_byte(word *buffer, size_t *byte, size_t *bit, word value)
{
	size_t i;

	for (i = 7; i < 8; --i)
		append_bit(buffer, byte, bit, (value >> i) & 1);
}

void
qr_encode_data(qr_code *qr, const char *text)
{
	const size_t length = strlen(text);
	size_t character_count_bits;
	size_t i, j, byte = 0, bit = 0;

	assert(qr->mode >= qr_detect_mode(text) &&
		"Text provided contains characters that are not valid for the specified mode");

	assert(length <= CAPACITY[qr->mode][qr->level][qr->version] &&
		"Text provided is too large");

	switch (qr->mode)
	{
	case QR_MODE_NUMERIC:
		// numeric mode indicator
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 1);

		// character count indicator
		character_count_bits =
			qr->version + 1 <=  9 ? 10 :
			qr->version + 1 <= 26 ? 12 : 14;
		for (i = 0; i < character_count_bits; ++i)
			append_bit(qr->codewords, &byte, &bit, (length >> (character_count_bits - 1 - i)) & 1);

		// data
		for (i = 0; i < length / 3; ++i)
		{
			unsigned value =
				100 * numeric_value(text[i * 3 + 0]) +
				 10 * numeric_value(text[i * 3 + 1]) +
				  1 * numeric_value(text[i * 3 + 2]);
			for (j = 0; j < 10; ++j)
				append_bit(qr->codewords, &byte, &bit, (value >> (9 - j)) & 1);
		}
		if (length % 3 == 2)
		{
			unsigned value =
				10 * numeric_value(text[i * 3 + 0]) +
				 1 * numeric_value(text[i * 3 + 1]);
			for (j = 0; j < 7; ++j)
				append_bit(qr->codewords, &byte, &bit, (value >> (6 - j)) & 1);
		}
		else if (length % 3 == 1)
		{
			unsigned value = numeric_value(text[i * 3]);
			for (j = 0; j < 4; ++j)
				append_bit(qr->codewords, &byte, &bit, (value >> (3 - j)) & 1);
		}
		break;

	case QR_MODE_ALPHANUMERIC:
		// alphanumeric mode indicator
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 1);
		append_bit(qr->codewords, &byte, &bit, 0);

		// character count indicator
		character_count_bits =
			qr->version + 1 <=  9 ? 9 :
			qr->version + 1 <= 26 ? 11 : 13;
		for (i = 0; i < character_count_bits; ++i)
			append_bit(qr->codewords, &byte, &bit, (length >> (character_count_bits - 1 - i)) & 1);

		// data
		for (i = 0; i < length / 2; ++i)
		{
			unsigned value =
				45 * alphanumeric_value(text[i * 2 + 0]) +
				 1 * alphanumeric_value(text[i * 2 + 1]);
			for (j = 0; j < 11; ++j)
				append_bit(qr->codewords, &byte, &bit, (value >> (10 - j)) & 1);
		}
		if (length % 2 == 1)
		{
			unsigned value = alphanumeric_value(text[i * 2]);
			for (j = 0; j < 6; ++j)
				append_bit(qr->codewords, &byte, &bit, (value >> (5 - j)) & 1);
		}
		break;

	case QR_MODE_BYTE:
		// byte mode indicator
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 1);
		append_bit(qr->codewords, &byte, &bit, 0);
		append_bit(qr->codewords, &byte, &bit, 0);

		// character count indicator
		character_count_bits = qr->version + 1 <= 9 ? 8 : 16;
		for (i = 0; i < character_count_bits; ++i)
			append_bit(qr->codewords, &byte, &bit, (length >> (character_count_bits - 1 - i)) & 1);

		// data
		for (i = 0; i < length; ++i)
			append_byte(qr->codewords, &byte, &bit, text[i]);
		break;

	default:
		assert(!"Specified encoding mode is not implemented");
	}

	// terminator
	for (i = 0; i < 4; ++i)
	{
		if (byte >= TOTAL_DATA_CODEWORD_COUNT[qr->level][qr->version]) break;
		append_bit(qr->codewords, &byte, &bit, 0);
	}

	// padding
	while (bit % 8)
		append_bit(qr->codewords, &byte, &bit, 0);
	for (i = 0; byte < TOTAL_DATA_CODEWORD_COUNT[qr->level][qr->version]; ++i)
		append_byte(qr->codewords, &byte, &bit, i % 2 == 0 ? 0xEC : 0x11);
}
