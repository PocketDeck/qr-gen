#ifndef QR_TYPES_H
#define QR_TYPES_H

#include <stddef.h>
#include <stdint.h>

#define QR_VERSION_COUNT 40

typedef enum
{
	QR_EC_LEVEL_L = 0,
	QR_EC_LEVEL_M,
	QR_EC_LEVEL_Q,
	QR_EC_LEVEL_H,
	QR_EC_LEVEL_COUNT
} qr_ecl;

typedef enum
{
	QR_MODE_NUMERIC = 0,
	QR_MODE_ALPHANUMERIC,
	QR_MODE_BYTE,
	QR_MODE_COUNT
} qr_mode;

typedef enum
{
	QR_MODULE_LIGHT = 0,
	QR_MODULE_DARK  = 1,
} qr_module;

#define QR_MASK_PATTERN_COUNT 8

typedef uint8_t word;

typedef struct
{
	unsigned version;
	qr_ecl level;
	qr_mode mode;

	qr_module *matrix;
	size_t side_length;

	unsigned mask;

	word *codewords;
	size_t codeword_count;
} qr_code;

#endif // QR_TYPES_H
