#ifndef QR_MATRIX_H
#define QR_MATRIX_H

#include <qr/types.h>
#include <stddef.h>
#include <stdio.h>

qr_module qr_matrix_get(const qr_code *qr, size_t i, size_t j);
void qr_matrix_set(qr_code *qr, size_t i, size_t j, qr_module value);
bool qr_matrix_is_reserved(const qr_code *qr, size_t i, size_t j);
void qr_matrix_place_codewords(qr_code *qr);

#endif // QR_MATRIX_H
