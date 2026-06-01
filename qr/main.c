#include <qr/enc.h>
#include <qr/matrix.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
log_(const char *fmt, ...)
#ifdef NDEBUG
{ (void) fmt; }
#else
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fflush(stderr);
}
#endif

static void
print_usage(const char *program_name)
{
	log_("Usage: %s <string> [error_correction [out_format]]\n", program_name);
	log_("  error_correction: L (7%%), M (15%%), Q (25%%), H (30%%). Default: M\n");
	log_("  out_format: PBM, SVG, TERM\n");
}

static qr_ecl
parse_ec_level(const char *level_str)
{
	if (!strcmp(level_str, "L")) return QR_EC_LEVEL_L;
	if (!strcmp(level_str, "M")) return QR_EC_LEVEL_M;
	if (!strcmp(level_str, "Q")) return QR_EC_LEVEL_Q;
	if (!strcmp(level_str, "H")) return QR_EC_LEVEL_H;

	log_("Warn: Invalid error correction level %s, using 'M'\n", level_str);
	return QR_EC_LEVEL_M;
}

static enum out_format
{
	FMT_PBM,
	FMT_SVG,
	FMT_TERM,
}
parse_out_format(const char *format_str)
{
	if (!strcmp(format_str, "PBM")) return FMT_PBM;
	if (!strcmp(format_str, "SVG")) return FMT_SVG;
	if (!strcmp(format_str, "TERM")) return FMT_TERM;

	log_("Warn: Invalid output format %s, using 'TERM'\n", format_str);
	return FMT_TERM;
}

int
main(int argc, char **argv)
{
	if (argc < 2)
	{
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	const char *input = argv[1];
	qr_ecl ec_level = (argc > 2) ? parse_ec_level(argv[2]) : QR_EC_LEVEL_M;
	enum out_format format = (argc > 3) ? parse_out_format(argv[3]) : FMT_TERM;
	qr_mode mode = qr_detect_mode(input);
	unsigned version = qr_min_version(mode, ec_level, strlen(input));
	if (!version)
	{
		log_("Error: Input too large for QR code\n");
		return EXIT_FAILURE;
	}

	log_("QR Code Generation:\n");
	log_("  Input: %s\n", input);
	log_("  Mode: %s\n", (const char *[]) { "Numeric", "Alphanumeric", "Byte" }[mode]);
	log_("  Error Correction: %s\n", (const char *[]) { "L (7%)", "M (15%)", "Q (25%)", "H (30%)" }[ec_level]);
	log_("  Version: %u\n", version);
	log_("\n");

	qr_code *qr = qr_create(version, mode, ec_level);
	if (!qr)
	{
		log_("Error: Memory allocation for QR code failed\n");
		return EXIT_FAILURE;
	}

	qr_encode_text(qr, input);
	log_("\n");

	switch (format)
	{
	case FMT_PBM:  qr_pbm_print(qr, stdout); break;
	case FMT_SVG:  qr_svg_print(qr, stdout); break;
	case FMT_TERM: qr_matrix_print(qr, stdout); break;
	}

	qr_destroy(qr);
	return EXIT_SUCCESS;
}
