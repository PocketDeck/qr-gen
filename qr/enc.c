#include <assert.h>
#include <qr/enc.h>
#include <qr/types.h>
#include <stddef.h>
#include <string.h>

static const size_t CAPACITY[QR_MODE_COUNT][QR_EC_LEVEL_COUNT][QR_VERSION_COUNT] =
{
	{ // Numeric
		{ // L
			  41,   77,  127,  187,  255,  322,  370,  461,  552,  652,
			 772,  883, 1022, 1101, 1250, 1408, 1548, 1725, 1903, 2061,
			2232, 2409, 2620, 2812, 3057, 3283, 3517, 3669, 3909, 4158,
			4417, 4686, 4965, 5253, 5529, 5836, 6153, 6479, 6743, 7089,
		},
		{ // M
			  34,   63,  101,  149,  202,  255,  293,  365,  432,  513,
			 604,  691,  796,  871,  991, 1082, 1212, 1346, 1500, 1600,
			1708, 1872, 2059, 2188, 2395, 2544, 2701, 2857, 3035, 3289,
			3486, 3693, 3909, 4134, 4343, 4588, 4775, 5039, 5313, 5596,
		},
		{ // Q
			  27,   48,   77,  111,  144,  178,  207,  259,  312,  364,
			 427,  489,  580,  621,  703,  775,  876,  948, 1063, 1159,
			1224, 1358, 1468, 1588, 1718, 1804, 1933, 2085, 2181, 2358,
			2473, 2670, 2805, 2949, 3081, 3244, 3417, 3599, 3791, 3993,
		},
		{ // H
			  17,   34,   58,   82,  106,  139,  154,  202,  235,  288,
			 331,  374,  427,  468,  530,  602,  674,  746,  813,  919,
			 969, 1056, 1108, 1228, 1286, 1425, 1501, 1581, 1677, 1782,
			1897, 2022, 2157, 2301, 2361, 2524, 2625, 2735, 2927, 3057,
		}
	},
	{ // Alphanumeric
		{ // L
			  25,   47,   77,  114,  154,  195,  224,  279,  335,  395,
			 468,  535,  619,  667,  758,  854,  938, 1046, 1153, 1249,
			1352, 1460, 1588, 1704, 1853, 1990, 2132, 2223, 2369, 2520,
			2677, 2840, 3009, 3183, 3351, 3537, 3729, 3927, 4087, 4296,
		},
		{ // M
			  20,   38,   61,   90,  122,  154,  178,  221,  262,  311,
			 366,  419,  483,  528,  600,  656,  734,  816,  909,  970,
			1035, 1134, 1248, 1326, 1451, 1542, 1637, 1732, 1839, 1994,
			2113, 2238, 2369, 2506, 2632, 2780, 2894, 3054, 3220, 3391,
		},
		{ // Q
			  16,   29,   47,   67,   87,  108,  125,  157,  189,  221,
			 259,  296,  352,  376,  426,  470,  531,  574,  644,  702,
			 742,  823,  890,  963, 1041, 1094, 1172, 1263, 1322, 1429,
			1499, 1618, 1700, 1787, 1867, 1966, 2071, 2181, 2298, 2420,
		},
		{ // H
			  10,   20,   35,   50,   64,   84,   93,  122,  143,  174,
			 200,  227,  259,  283,  321,  365,  408,  452,  493,  557,
			 587,  640,  672,  744,  779,  864,  910,  958, 1016, 1080,
			1150, 1226, 1307, 1394, 1431, 1530, 1591, 1658, 1774, 1852,
		}
	},
	{ // Byte
		{ // L
			  17,   32,   53,   78,  106,  134,  154,  192,  230,  271,
			 321,  367,  425,  458,  520,  586,  644,  718,  792,  858,
			 929, 1003, 1091, 1171, 1273, 1367, 1465, 1528, 1628, 1732,
			1840, 1952, 2068, 2188, 2303, 2431, 2563, 2699, 2809, 2953,
		},
		{ // M
			  14,   26,   42,   62,   84,  106,  122,  152,  180,  213,
			 251,  287,  331,  362,  412,  450,  504,  560,  624,  666,
			 711,  779,  857,  911,  997, 1059, 1125, 1190, 1264, 1370,
			1452, 1538, 1628, 1722, 1809, 1911, 1989, 2099, 2213, 2331,
		},
		{ // Q
			  11,   20,   32,   46,   60,   74,   86,  108,  130,  151,
			 177,  203,  241,  258,  292,  322,  364,  394,  442,  482,
			 509,  565,  611,  661,  715,  751,  805,  868,  908,  982,
			1030, 1112, 1168, 1228, 1283, 1351, 1423, 1499, 1579, 1663,
		},
		{ // H
			   7,   14,   24,   34,   44,   58,   64,   84,   98,  119,
			 137,  155,  177,  194,  220,  250,  280,  310,  338,  382,
			 403,  439,  461,  511,  535,  593,  625,  658,  698,  742,
			 790,  842,  898,  958,  983, 1051, 1093, 1139, 1219, 1273,
		}
	}
};

unsigned
qr_min_version(qr_mode mode, qr_ecl level, size_t bytes)
{
	size_t i;

	for (i = 0; i < QR_VERSION_COUNT && bytes > CAPACITY[mode][level][i]; ++i);

	return i == QR_VERSION_COUNT ? 0 : (unsigned) (i + 1);
}

#define INVALID_VALUE 0x100

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
	append_bit(qr->codewords, &byte, &bit, 0);
	append_bit(qr->codewords, &byte, &bit, 0);
	append_bit(qr->codewords, &byte, &bit, 0);
	append_bit(qr->codewords, &byte, &bit, 0);

	// padding
	while (bit % 8)
		append_bit(qr->codewords, &byte, &bit, 0);
	for (i = 0; i < CAPACITY[qr->mode][qr->level][qr->version] - length; ++i)
		append_byte(qr->codewords, &byte, &bit, i % 2 == 0 ? 0xEC : 0x11);
}
