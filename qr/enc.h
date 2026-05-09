#ifndef QR_ENC_H
#define QR_ENC_H

#include <qr/types.h>
#include <stddef.h>

unsigned qr_min_version(qr_encoding_mode mode, qr_ec_level level, size_t bytes);
void qr_encode_data(qr_code *qr, const char *message);

#endif // QR_ENC_H
