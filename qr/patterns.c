#include <qr/matrix.h>
#include <qr/patterns.h>
#include <qr/tables.h>
#include <qr/types.h>
#include <stddef.h>

static void
add_finder_pattern_at(qr_code *qr, size_t i, size_t j)
{
	size_t di, dj;

	for (di = 0; di < 7; ++di)
		for (dj = 0; dj < 7; ++dj)
			qr_module_set(qr, i + di, j + dj, QR_MODULE_DARK);

	for (di = 1; di < 6; ++di)
		for (dj = 1; dj < 6; ++dj)
			qr_module_set(qr, i + di, j + dj, QR_MODULE_LIGHT);

	for (di = 2; di < 5; ++di)
		for (dj = 2; dj < 5; ++dj)
			qr_module_set(qr, i + di, j + dj, QR_MODULE_DARK);
}

void
qr_finder_patterns_apply(qr_code *qr)
{
	add_finder_pattern_at(qr, 0, 0);
	add_finder_pattern_at(qr, qr->side_length - 7, 0);
	add_finder_pattern_at(qr, 0, qr->side_length - 7);
}

void
qr_separators_apply(qr_code *qr)
{
	size_t i;

	for (i = 0; i < 8; ++i)
	{
		// upper left
		qr_module_set(qr, i, 7, QR_MODULE_LIGHT);
		qr_module_set(qr, 7, i, QR_MODULE_LIGHT);

		// upper right
		qr_module_set(qr, i, qr->side_length - 8, QR_MODULE_LIGHT);
		qr_module_set(qr, 7, qr->side_length - 8 + i, QR_MODULE_LIGHT);

		// lower left
		qr_module_set(qr, qr->side_length - 8 + i, 7, QR_MODULE_LIGHT);
		qr_module_set(qr, qr->side_length - 8, i, QR_MODULE_LIGHT);
	}
}

void
qr_timing_patterns_apply(qr_code *qr)
{
	size_t i;

	for (i = 8; i < qr->side_length - 8; ++i)
	{
		qr_module_set(qr, i, 6, (i % 2) ^ 1);
		qr_module_set(qr, 6, i, (i % 2) ^ 1);
	}
}

static void
add_alignment_pattern_at(qr_code *qr, size_t i, size_t j)
{
	size_t di, dj;

	for (di = 0; di < 5; ++di)
		for (dj = 0; dj < 5; ++dj)
			qr_module_set(qr, i + di, j + dj, QR_MODULE_DARK);

	for (di = 1; di < 4; ++di)
		for (dj = 1; dj < 4; ++dj)
			qr_module_set(qr, i + di, j + dj, QR_MODULE_LIGHT);

	qr_module_set(qr, i + 2, j + 2, QR_MODULE_DARK);
}

void
qr_alignment_patterns_apply(qr_code *qr)
{
	size_t entry_i, entry_j, i, j;
	bool in_finder_upper_left, in_finder_upper_right, in_finder_lower_left;

	for (entry_i = 0; entry_i < MAX_ALIGNMENT_ENTRIES; ++entry_i)
	{
		for (entry_j = 0; entry_j < MAX_ALIGNMENT_ENTRIES; ++entry_j)
		{
			i = ALIGNMENT_CENTER_MODULE[qr->version][entry_i] - 2;
			j = ALIGNMENT_CENTER_MODULE[qr->version][entry_j] - 2;

			in_finder_upper_left = i < 8 && j < 8;
			in_finder_upper_right = i < 8 && j >= qr->side_length - 12;
			in_finder_lower_left = i >= qr->side_length - 12 && j < 8;

			if (!i || !j || in_finder_upper_left || in_finder_upper_right || in_finder_lower_left)
				continue;

			add_alignment_pattern_at(qr, i, j);
		}
	}
}

bool
qr_is_in_alignment_patterns(const qr_code *qr, size_t i, size_t j)
{
	size_t entry_i, entry_j, center_i, center_j;
	bool in_finder_upper_left, in_finder_upper_right, in_finder_lower_left;

	// find closest entry on each axis
	for (entry_i = 0; entry_i < MAX_ALIGNMENT_ENTRIES - 1; ++entry_i)
	{
		if (ALIGNMENT_CENTER_MODULE[qr->version][entry_i + 1] == E) break;

		if (ALIGNMENT_CENTER_MODULE[qr->version][entry_i + 1] >= i)
		{
			if (ALIGNMENT_CENTER_MODULE[qr->version][entry_i] >= i) break;
			if (ALIGNMENT_CENTER_MODULE[qr->version][entry_i + 1] - i < i - ALIGNMENT_CENTER_MODULE[qr->version][entry_i])
				++entry_i;
			break;
		}
	}

	for (entry_j = 0; entry_j < MAX_ALIGNMENT_ENTRIES - 1; ++entry_j)
	{
		if (ALIGNMENT_CENTER_MODULE[qr->version][entry_j + 1] == E) break;

		if (ALIGNMENT_CENTER_MODULE[qr->version][entry_j + 1] >= j)
		{
			if (ALIGNMENT_CENTER_MODULE[qr->version][entry_j] >= j) break;
			if (ALIGNMENT_CENTER_MODULE[qr->version][entry_j + 1] - j < j - ALIGNMENT_CENTER_MODULE[qr->version][entry_j])
				++entry_j;
			break;
		}
	}

	center_i = ALIGNMENT_CENTER_MODULE[qr->version][entry_i];
	center_j = ALIGNMENT_CENTER_MODULE[qr->version][entry_j];

	in_finder_upper_left = center_i == 6 && center_j == 6;
	in_finder_upper_right = center_i == 6 && center_j == qr->side_length - 7;
	in_finder_lower_left = center_i == qr->side_length - 7 && center_j == 6;

	if (in_finder_upper_left || in_finder_upper_right || in_finder_lower_left) return false;

	return i >= center_i - 2 && i <= center_i + 2 && j >= center_j - 2 && j <= center_j + 2;
}
