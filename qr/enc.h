#ifndef QR_ENC_H
#define QR_ENC_H

#include <qr/types.h>
#include <stddef.h>

unsigned qr_min_version(qr_mode mode, qr_ecl level, size_t bytes);
void qr_encode_data(qr_code *qr, const char *message);

#endif // QR_ENC_H
