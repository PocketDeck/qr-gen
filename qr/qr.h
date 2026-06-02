#ifndef QR_QR_H
#define QR_QR_H

#include <qr/types.h>
#include <stdio.h>

qr_code *qr_create(unsigned version, qr_mode mode, qr_ecl level);
void qr_destroy(qr_code *qr);
void qr_build(qr_code *qr, const char *text);

void qr_print_matrix(const qr_code *qr, FILE *stream);
void qr_print_pbm(const qr_code *qr, FILE *stream);
void qr_print_svg(const qr_code *qr, FILE *stream);

#endif // QR_QR_H
