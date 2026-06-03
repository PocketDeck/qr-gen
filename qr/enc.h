#ifndef QR_ENC_H
#define QR_ENC_H

#include <qr/types.h>
#include <stddef.h>

qr_mode qr_detect_mode(const char *text);
unsigned qr_detect_version(qr_mode mode, qr_ecl level, size_t bytes);
void qr_encode_data(qr_code *qr, const char *text);

#endif // QR_ENC_H
