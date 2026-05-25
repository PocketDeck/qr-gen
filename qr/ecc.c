#include <assert.h>
#include <qr/ecc.h>
#include <qr/tables.h>
#include <qr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

constexpr size_t GF_SIZE = 256;
constexpr unsigned PRIMITIVE = 0x11D;

static word gf_log[GF_SIZE];
static word gf_antilog[(GF_SIZE * 2) - 2];
static bool gf_tables_initialized = false;

static void
gf_init_log_antilog(void)
{
	if (gf_tables_initialized) return;

	size_t i;
	word x = 1;

	for (i = 0; i < GF_SIZE - 1; ++i)
	{
		gf_antilog[i] = x;
		gf_antilog[i + (GF_SIZE - 1)] = x;
		gf_log[x] = (word) i;
		x = (word) ((x << 1) ^ ((x & 0x80) ? PRIMITIVE : 0));
	}

	gf_tables_initialized = true;
}

static inline word
gf_mul(word a, word b)
{
	if (a == 0 || b == 0) return 0;
	return gf_antilog[gf_log[a] + gf_log[b]];
}

static inline word
gf_add(word a, word b)
{
	return a ^ b;
}

static void
generator_polynomial(word *poly, size_t degree)
{
	if (degree == 0) return;

	size_t i, j;
	word coef;

	for (i = 0; i < degree - 1; ++i)
		poly[i] = 0;
	poly[degree - 1] = 1;

	for (i = 0; i < degree; ++i)
	{
		coef = gf_antilog[i];
		for (j = 0; j < degree - 1; ++j)
			poly[j] = gf_add(gf_mul(poly[j], coef), poly[j + 1]);
		poly[degree - 1] = gf_mul(poly[degree - 1], coef);
	}
}

static void
ecc_generate(const word *data, size_t data_length, word *ecc, size_t ecc_length, word g[ecc_length])
{
	size_t i, j;
	word feedback;

	for (i = 0; i < ecc_length; ++i)
		ecc[i] = 0;

	for (i = 0; i < data_length; ++i)
	{
		feedback = gf_add(data[i], ecc[0]);
		for (j = 0; j < ecc_length - 1; ++j)
			ecc[j] = gf_add(ecc[j + 1], gf_mul(g[j], feedback));
		ecc[ecc_length - 1] = gf_mul(g[ecc_length - 1], feedback);
	}
}

void
qr_ec_encode(qr_code *qr)
{
	gf_init_log_antilog();

	size_t i, j, data_length, ecc_length;
	word generator[68];  // maximal size of ecc
	word *data = qr->codewords;
	word *ecc = qr->codewords + TOTAL_DATA_CODEWORD_COUNT[qr->level][qr->version];

	for (i = 0; i < BLOCK_TYPES_PER_VERSION; ++i)
	{
		data_length = DATA_CODEWORD_COUNT[qr->level][qr->version][i];
		ecc_length = TOTAL_CODEWORD_COUNT[qr->level][qr->version][i] - data_length;

		if (ecc_length == 0) continue;

		generator_polynomial(generator, ecc_length);

		for (j = 0; j < BLOCK_COUNT[qr->level][qr->version][i]; ++j)
		{
			ecc_generate(data, data_length, ecc, ecc_length, generator);
			data += data_length;
			ecc += ecc_length;
		}
	}

	assert(data - qr->codewords == (ptrdiff_t) TOTAL_DATA_CODEWORD_COUNT[qr->level][qr->version] &&
		"Sum of data codewords in blocks do not match expected number of data codewords");
	assert(ecc - qr->codewords == (ptrdiff_t) qr->codeword_count &&
		"Number of generated ec codewords do not match the expected number of codewords");
}

static word *
interleave_words(const size_t codeword_count[BLOCK_TYPES_PER_VERSION], const size_t block_count[BLOCK_TYPES_PER_VERSION], word *in, word *out)
{
	size_t i, block, codeword;
	size_t block_offset[BLOCK_TYPES_PER_VERSION], max_codeword_count = 0;

	for (i = 0; i < BLOCK_TYPES_PER_VERSION; ++i)
	{
		block_offset[i] = i ? block_offset[i - 1] + (codeword_count[i - 1] * block_count[i - 1]) : 0;
		if (codeword_count[i] > max_codeword_count)
			max_codeword_count = codeword_count[i];
	}

	for (codeword = 0; codeword < max_codeword_count; ++codeword)
	{
		for (i = 0; i < BLOCK_TYPES_PER_VERSION; ++i)
		{
			if (codeword >= codeword_count[i]) continue;

			for (block = 0; block < block_count[i]; ++block)
				*(out++) = in[(block * codeword_count[i]) + codeword + block_offset[i]];
		}
	}

	return out;
}

void
qr_interleave_codewords(qr_code *qr)
{
	const size_t *data_codeword_count = DATA_CODEWORD_COUNT[qr->level][qr->version];
	const size_t *block_count = BLOCK_COUNT[qr->level][qr->version];
	size_t i, ecc_codeword_count[BLOCK_TYPES_PER_VERSION];
	word final_codewords[qr->codeword_count], *word_ptr = final_codewords;

	for (i = 0; i < BLOCK_TYPES_PER_VERSION; ++i)
		ecc_codeword_count[i] = TOTAL_CODEWORD_COUNT[qr->level][qr->version][i] - data_codeword_count[i];

	word_ptr = interleave_words(data_codeword_count, block_count, qr->codewords, word_ptr);
	word_ptr = interleave_words(ecc_codeword_count, block_count, qr->codewords + TOTAL_DATA_CODEWORD_COUNT[qr->level][qr->version], word_ptr);

	assert(word_ptr == final_codewords + qr->codeword_count &&
		"Length of interleaved codewords does not match length of original codewords");

	for (i = 0; i < qr->codeword_count; ++i)
		qr->codewords[i] = final_codewords[i];
}
