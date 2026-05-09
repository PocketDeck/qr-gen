#ifndef QR_TYPES_H
#define QR_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
	QR_EC_LEVEL_L = 0,
	QR_EC_LEVEL_M,
	QR_EC_LEVEL_Q,
	QR_EC_LEVEL_H,
	QR_EC_LEVEL_COUNT
} qr_ecl;

#define QR_VERSION_COUNT 40

typedef enum
{
	QR_MODE_NUMERIC,
	QR_MODE_ALPHANUMERIC,
	QR_MODE_BYTE,
} qr_mode;

typedef uint8_t word;

typedef enum
{
	QR_MODULE_LIGHT = 0,
	QR_MODULE_DARK  = 1,
} qr_module;


typedef struct
{
	qr_ecl level;
	qr_mode mode;
	unsigned version;

	qr_module *matrix;
	size_t side_length;

	unsigned mask;

	size_t codeword_count;
	word *codewords;
} qr_code;

#endif // QR_TYPES_H
