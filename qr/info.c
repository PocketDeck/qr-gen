#include <qr/info.h>
#include <qr/matrix.h>
#include <qr/tables.h>
#include <qr/types.h>
#include <stddef.h>

static const unsigned ECL_INDICATOR_MAP[QR_EC_LEVEL_COUNT] =
{
	[QR_EC_LEVEL_L]=8,
	[QR_EC_LEVEL_M]=0,
	[QR_EC_LEVEL_Q]=24,
	[QR_EC_LEVEL_H]=16,
};

void
qr_format_info_apply(qr_code *qr)
{
	unsigned format_info = FORMAT_INFO_MAP[ECL_INDICATOR_MAP[qr->level] + qr->mask];

	// upper left
	qr_module_set(qr, 0, 8, (format_info >> 0) & 1);
	qr_module_set(qr, 1, 8, (format_info >> 1) & 1);
	qr_module_set(qr, 2, 8, (format_info >> 2) & 1);
	qr_module_set(qr, 3, 8, (format_info >> 3) & 1);
	qr_module_set(qr, 4, 8, (format_info >> 4) & 1);
	qr_module_set(qr, 5, 8, (format_info >> 5) & 1);
	qr_module_set(qr, 7, 8, (format_info >> 6) & 1);
	qr_module_set(qr, 8, 8, (format_info >> 7) & 1);
	qr_module_set(qr, 8, 7, (format_info >> 8) & 1);
	qr_module_set(qr, 8, 5, (format_info >> 9) & 1);
	qr_module_set(qr, 8, 4, (format_info >> 10) & 1);
	qr_module_set(qr, 8, 3, (format_info >> 11) & 1);
	qr_module_set(qr, 8, 2, (format_info >> 12) & 1);
	qr_module_set(qr, 8, 1, (format_info >> 13) & 1);
	qr_module_set(qr, 8, 0, (format_info >> 14) & 1);

	// upper right
	qr_module_set(qr, 8, qr->side_length - 1, (format_info >> 0) & 1);
	qr_module_set(qr, 8, qr->side_length - 2, (format_info >> 1) & 1);
	qr_module_set(qr, 8, qr->side_length - 3, (format_info >> 2) & 1);
	qr_module_set(qr, 8, qr->side_length - 4, (format_info >> 3) & 1);
	qr_module_set(qr, 8, qr->side_length - 5, (format_info >> 4) & 1);
	qr_module_set(qr, 8, qr->side_length - 6, (format_info >> 5) & 1);
	qr_module_set(qr, 8, qr->side_length - 7, (format_info >> 6) & 1);
	qr_module_set(qr, 8, qr->side_length - 8, (format_info >> 7) & 1);

	// lower left
	qr_module_set(qr, qr->side_length - 8, 8, QR_MODULE_DARK);
	qr_module_set(qr, qr->side_length - 7, 8, (format_info >> 8) & 1);
	qr_module_set(qr, qr->side_length - 6, 8, (format_info >> 9) & 1);
	qr_module_set(qr, qr->side_length - 5, 8, (format_info >> 10) & 1);
	qr_module_set(qr, qr->side_length - 4, 8, (format_info >> 11) & 1);
	qr_module_set(qr, qr->side_length - 3, 8, (format_info >> 12) & 1);
	qr_module_set(qr, qr->side_length - 2, 8, (format_info >> 13) & 1);
	qr_module_set(qr, qr->side_length - 1, 8, (format_info >> 14) & 1);
}

void
qr_version_info_apply(qr_code *qr)
{
	unsigned version_info = VERSION_INFO_MAP[qr->version];
	if (!version_info) return;

	// upper right
	qr_module_set(qr, 0, qr->side_length - 11, (version_info >> 0) & 1);
	qr_module_set(qr, 0, qr->side_length - 10, (version_info >> 1) & 1);
	qr_module_set(qr, 0, qr->side_length -  9, (version_info >> 2) & 1);
	qr_module_set(qr, 1, qr->side_length - 11, (version_info >> 3) & 1);
	qr_module_set(qr, 1, qr->side_length - 10, (version_info >> 4) & 1);
	qr_module_set(qr, 1, qr->side_length -  9, (version_info >> 5) & 1);
	qr_module_set(qr, 2, qr->side_length - 11, (version_info >> 6) & 1);
	qr_module_set(qr, 2, qr->side_length - 10, (version_info >> 7) & 1);
	qr_module_set(qr, 2, qr->side_length -  9, (version_info >> 8) & 1);
	qr_module_set(qr, 3, qr->side_length - 11, (version_info >> 9) & 1);
	qr_module_set(qr, 3, qr->side_length - 10, (version_info >> 10) & 1);
	qr_module_set(qr, 3, qr->side_length -  9, (version_info >> 11) & 1);
	qr_module_set(qr, 4, qr->side_length - 11, (version_info >> 12) & 1);
	qr_module_set(qr, 4, qr->side_length - 10, (version_info >> 13) & 1);
	qr_module_set(qr, 4, qr->side_length -  9, (version_info >> 14) & 1);
	qr_module_set(qr, 5, qr->side_length - 11, (version_info >> 15) & 1);
	qr_module_set(qr, 5, qr->side_length - 10, (version_info >> 16) & 1);
	qr_module_set(qr, 5, qr->side_length -  9, (version_info >> 17) & 1);

	// lower left
	qr_module_set(qr, qr->side_length - 11, 0, (version_info >> 0) & 1);
	qr_module_set(qr, qr->side_length - 10, 0, (version_info >> 1) & 1);
	qr_module_set(qr, qr->side_length -  9, 0, (version_info >> 2) & 1);
	qr_module_set(qr, qr->side_length - 11, 1, (version_info >> 3) & 1);
	qr_module_set(qr, qr->side_length - 10, 1, (version_info >> 4) & 1);
	qr_module_set(qr, qr->side_length -  9, 1, (version_info >> 5) & 1);
	qr_module_set(qr, qr->side_length - 11, 2, (version_info >> 6) & 1);
	qr_module_set(qr, qr->side_length - 10, 2, (version_info >> 7) & 1);
	qr_module_set(qr, qr->side_length -  9, 2, (version_info >> 8) & 1);
	qr_module_set(qr, qr->side_length - 11, 3, (version_info >> 9) & 1);
	qr_module_set(qr, qr->side_length - 10, 3, (version_info >> 10) & 1);
	qr_module_set(qr, qr->side_length -  9, 3, (version_info >> 11) & 1);
	qr_module_set(qr, qr->side_length - 11, 4, (version_info >> 12) & 1);
	qr_module_set(qr, qr->side_length - 10, 4, (version_info >> 13) & 1);
	qr_module_set(qr, qr->side_length -  9, 4, (version_info >> 14) & 1);
	qr_module_set(qr, qr->side_length - 11, 5, (version_info >> 15) & 1);
	qr_module_set(qr, qr->side_length - 10, 5, (version_info >> 16) & 1);
	qr_module_set(qr, qr->side_length -  9, 5, (version_info >> 17) & 1);
}
