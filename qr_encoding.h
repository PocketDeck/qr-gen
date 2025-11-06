#ifndef QR_ENCODING_H
#define QR_ENCODING_H

#include "qr_types.h"

// Function to get the minimum version needed for a given string length and error correction level
unsigned qr_min_version(const char *str, qr_ec_level level);

#endif // QR_ENCODING_H
