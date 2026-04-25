#include <assert.h>
#include <qr/matrix.h>
#include <qr/patterns.h>
#include <qr/types.h>
#include <stddef.h>
#include <stdio.h>

qr_module_state
qr_module_get(const qr_code *qr, size_t i, size_t j)
{
	return qr->matrix[i * qr->side_length + j] ? QR_MODULE_DARK : QR_MODULE_LIGHT;
}

void
qr_module_set(qr_code *qr, size_t i, size_t j, qr_module_state value)
{
	qr->matrix[i * qr->side_length + j] = value;
}

void
qr_matrix_print(const qr_code *qr, FILE *stream)
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
			fprintf(stream, "%s", qr_module_get(qr, i, j) ? "  " : "\x1b[7m  \x1b[27m");

		// quiet zone
		fprintf(stream, "\x1b[7m        \x1b[27m");

		fprintf(stream, "\n");
	}

	// quiet zone
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < qr->side_length + 8; ++j)
			fprintf(stream, "\x1b[7m  \x1b[27m");
		fprintf(stream, "\n");
	}
}

bool
qr_module_is_reserved(const qr_code *qr, size_t i, size_t j)
{
	// finder pattern (7) + separator (1)
	bool in_finder_upper_left = i < 8 && j < 8;
	bool in_finder_upper_right = i < 8 && j >= qr->side_length - 8;
	bool in_finder_lower_left = i >= qr->side_length - 8 && j < 8;
	bool in_finder = in_finder_upper_left || in_finder_upper_right || in_finder_lower_left;

	bool in_timing = i == 6 || j == 6;
	bool in_alignment = qr_is_in_alignment_patterns(qr, i, j);

	bool in_version_lower_left = i < 6 && j >= qr->side_length - 11;
	bool in_version_upper_right = i >= qr->side_length - 11 && j < 6;
	bool in_version = qr->version + 1 >= 7 && (in_version_lower_left || in_version_upper_right);

	bool in_format_upper_left = i < 9 && j < 9;
	bool in_format_upper_right = i < 9 && j >= qr->side_length - 8;
	bool in_format_lower_left = i >= qr->side_length - 8 && j < 9;
	bool in_format = in_format_upper_left || in_format_upper_right || in_format_lower_left;

	return in_finder || in_timing || in_alignment || in_version || in_format;
}

static void
place_bit(qr_code *qr, size_t *i, size_t *j, bool *left, bool *up, qr_module_state value)
{
	bool exit = false;

	while (!exit)
	{
		if (!qr_module_is_reserved(qr, *i, *j))
		{
			qr_module_set(qr, *i, *j, value);
			exit = true;
		}

		if (!*left)
		{
			if ((*up && *i == 0) || (!*up && *i == qr->side_length - 1))
			{
				*up = !*up;
				*j -= 2;
			}
			else
			{
				*i += *up ? -1 : 1;
			}
			++*j;
		}
		else
		{
			--*j;
		}

		*left = !*left;

		// skip vertical timing pattern
		if (*j == 6) --*j;
	}
}

static const size_t REMAINDER_BITS[QR_VERSION_COUNT] =
{
	0, 7, 7, 7, 7, 7, 0, 0, 0, 0,
	0, 0, 0, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 3, 3, 3,
	3, 3, 3, 3, 0, 0, 0, 0, 0, 0,
};

void
qr_place_codewords(qr_code *qr)
{
	size_t word, bit;
	size_t i, j;
	bool left = true, up = true;
	i = j = qr->side_length - 1;

	for (word = 0; word < qr->codeword_count; ++word)
	{
		for (bit = 7; bit < 8; --bit)
			place_bit(qr, &i, &j, &left, &up, (qr->codewords[word] >> bit) & 1);
	}

	for (bit = 0; bit < REMAINDER_BITS[qr->version]; ++bit)
		place_bit(qr, &i, &j, &left, &up, 0);

	assert(i == qr->side_length - (qr->version + 1 >= 7 ? 11 : 8) && j == 1 && "Codewords do not fill symbol completely");
}
