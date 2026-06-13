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
	qr->codewords = malloc(sizeof(qr_word[qr->codeword_count]));
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
qr_build(qr_code *qr, const char *text)
{
	// 1. enc
	qr_encode_data(qr, text);
	// 2. ecc
	qr_ecc_encode(qr);
	// 3. block
	qr_ecc_interleave(qr);
	// 4. matrix
	qr_matrix_place_codewords(qr);
	qr_finder_patterns_apply(qr);
	qr_separators_apply(qr);
	qr_timing_patterns_apply(qr);
	qr_alignment_patterns_apply(qr);
	// 5. masking
	qr_mask_apply(qr);
	// 6. info
	qr_format_info_apply(qr);
	qr_version_info_apply(qr);
}

void
qr_print_matrix(const qr_code *qr, FILE *stream)
{
	size_t i, j;

	// quiet zone
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < qr->side_length + 8; ++j)
			fprintf(stream, "\x1b[7m  \x1b[27m");
		fprintf(stream, "\n");
	}

	for (i = 0; i < qr->side_length; ++i)
	{
		// quiet zone
		fprintf(stream, "\x1b[7m        \x1b[27m");

		for (j = 0; j < qr->side_length; ++j)
			fprintf(stream, "%s", qr_matrix_get(qr, i, j) ? "  " : "\x1b[7m  \x1b[27m");

		// quiet zone
		fprintf(stream, "\x1b[7m        \x1b[27m\n");
	}

	// quiet zone
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < qr->side_length + 8; ++j)
			fprintf(stream, "\x1b[7m  \x1b[27m");
		fprintf(stream, "\n");
	}
}

void
qr_print_pbm(const qr_code *qr, FILE *stream)
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
			fprintf(stream, "%u ", (unsigned) qr_matrix_get(qr, i, j));

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
qr_print_svg(const qr_code *qr, FILE *stream)
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
			color = qr_matrix_get(qr, i, j) ? "black" : "white";
			fprintf(stream, "<rect x=\"%zu\" y=\"%zu\" width=\"1\" height=\"1\" fill=\"%s\"/>\n", j + 4, i + 4, color);
		}
	}

	fprintf(stream, "</svg>\n");
}
