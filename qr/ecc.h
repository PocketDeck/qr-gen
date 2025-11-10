#ifndef QR_ECC_H
#define QR_ECC_H

#include <qr/types.h>

void qr_ec_encode(qr_code *qr);
void qr_interleave_codewords(qr_code *qr);

#endif // QR_ECC_H
