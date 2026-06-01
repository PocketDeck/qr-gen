#include <qr/ecc.h>
#include <qr/enc.h>
#include <qr/info.h>
#include <qr/mask.h>
#include <qr/matrix.h>
#include <qr/patterns.h>
#include <qr/qr.h>
#include <qr/tables.h>
#include <qr/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

extern void LOG(const char *fmt, ...);

qr_code *
qr_create(unsigned version, qr_mode mode, qr_ecl level)
{
	if (--version >= QR_VERSION_COUNT) return nullptr;

	qr_code *qr = malloc(sizeof(qr_code));
	if (!qr) return nullptr;

	qr->version = version;
	qr->mode = mode;
	qr->level = level;
	qr->side_length = 21 + (qr->version * 4);
	qr->matrix = malloc(sizeof(qr_module[qr->side_length * qr->side_length]));
	if (!qr->matrix)
	{
		free(qr);
		return nullptr;
	}

	qr->codeword_count = CODEWORD_COUNT[qr->version];
	qr->codewords = malloc(sizeof(word[qr->codeword_count]));
	if (!qr->codewords)
	{
		free(qr->matrix);
		free(qr);
		return nullptr;
	}

	return qr;
}

void
qr_destroy(qr_code *qr)
{
	free(qr->codewords);
	free(qr->matrix);
	free(qr);
}

void
qr_encode_text(qr_code *qr, const char *text)
{
	// 1. enc
	LOG("Encoding text...............");
	qr_encode_data(qr, text);
	LOG("OK\n");

	// 2. ecc
	LOG("Encoding error correction...");
	qr_ec_encode(qr);
	LOG("OK\n");

	// 3. block
	LOG("Interleaving codewords......");
	qr_interleave_codewords(qr);
	LOG("OK\n");

	// 4. matrix
	LOG("Generating matrix...........");
	qr_place_codewords(qr);
	qr_finder_patterns_apply(qr);
	qr_separators_apply(qr);
	qr_timing_patterns_apply(qr);
	qr_alignment_patterns_apply(qr);
	LOG("OK\n");

	// 5. masking
	LOG("Masking.....................");
	qr_mask_apply(qr);
	LOG("OK\n");

	// 6. info
	LOG("Applying meta information...");
	qr_format_info_apply(qr);
	qr_version_info_apply(qr);
	LOG("OK\n");
}

void
qr_pbm_print(qr_code *qr, FILE *stream)
{
	size_t i, j;
	char *fmt_header = "P1\n%zu %zu\n";
	size_t total_side_length = qr->side_length + 8;  // added quiet zone padding

	fprintf(stream, fmt_header, total_side_length, total_side_length);

	// quiet zone
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < total_side_length - 1; ++j)
			fprintf(stream, "0 ");
		fprintf(stream, "0\n");
	}

	for (i = 0; i < qr->side_length; ++i)
	{
		// quiet zone
		fprintf(stream, "0 0 0 0 ");

		for (j = 0; j < qr->side_length; ++j)
			fprintf(stream, "%u ", (unsigned) qr_module_get(qr, i, j));

		// quiet zone
		fprintf(stream, "0 0 0 0\n");
	}

	// quiet zone
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < total_side_length - 1; ++j)
			fprintf(stream, "0 ");
		fprintf(stream, "0\n");
	}
}

void
qr_svg_print(qr_code *qr, FILE *stream)
{
	size_t i, j;
	char *color;
	char *fmt_header =
		"<svg xmlns=\"http://www.w3.org/2000/svg\" "
		"width=\"%zu\" height=\"%zu\" viewBox=\"0 0 %zu %zu\" "
		"shape-rendering=\"crispEdges\">\n";
	size_t total_side_length = qr->side_length + 8;  // added quiet zone padding

	fprintf(stream, fmt_header, total_side_length, total_side_length, total_side_length, total_side_length);
	fprintf(stream, "<rect width=\"%zu\" height=\"%zu\" fill=\"white\"/>\n", total_side_length, total_side_length);

	for (i = 0; i < qr->side_length; ++i)
	{
		for (j = 0; j < qr->side_length; ++j)
		{
			color = qr_module_get(qr, i, j) ? "black" : "white";
			fprintf(stream, "<rect x=\"%zu\" y=\"%zu\" width=\"1\" height=\"1\" fill=\"%s\"/>\n", j + 4, i + 4, color);
		}
	}

	fprintf(stream, "</svg>\n");
}
