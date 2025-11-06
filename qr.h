#ifndef QR_H
#define QR_H

typedef struct qr_code* qr_code;

qr_code *qr_from_string(const char *str);
char *qr_create_svg_string(qr_code *qr);

#endif // QR_H