/**
 * @file qr.c
 * @brief QR code creation and encoding tests
 */

#include <qr/enc.h>
#include <qr/qr.h>
#include <qr/types.h>
#include <stddef.h>
#include <string.h>
#include <test/base.h>

/**
 * @brief Test all QR versions can be created and encoded
 */
static struct test_result
create_and_encode(unsigned version, qr_ecl level, qr_mode mode)
{
	const char *test_text = "123456";

	qr_code *qr = qr_create(version, mode, level);
	if (!qr) return TEST_FAILURE("Failed to create QR code");

	qr_build(qr, test_text);
	qr_destroy(qr);

	return TEST_SUCCESS;
}

TEST(create_and_encode_01_L_N) { return create_and_encode( 1, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_02_L_N) { return create_and_encode( 2, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_03_L_N) { return create_and_encode( 3, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_04_L_N) { return create_and_encode( 4, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_05_L_N) { return create_and_encode( 5, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_06_L_N) { return create_and_encode( 6, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_07_L_N) { return create_and_encode( 7, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_08_L_N) { return create_and_encode( 8, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_09_L_N) { return create_and_encode( 9, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_10_L_N) { return create_and_encode(10, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_11_L_N) { return create_and_encode(11, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_12_L_N) { return create_and_encode(12, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_13_L_N) { return create_and_encode(13, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_14_L_N) { return create_and_encode(14, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_15_L_N) { return create_and_encode(15, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_16_L_N) { return create_and_encode(16, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_17_L_N) { return create_and_encode(17, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_18_L_N) { return create_and_encode(18, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_19_L_N) { return create_and_encode(19, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_20_L_N) { return create_and_encode(20, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_21_L_N) { return create_and_encode(21, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_22_L_N) { return create_and_encode(22, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_23_L_N) { return create_and_encode(23, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_24_L_N) { return create_and_encode(24, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_25_L_N) { return create_and_encode(25, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_26_L_N) { return create_and_encode(26, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_27_L_N) { return create_and_encode(27, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_28_L_N) { return create_and_encode(28, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_29_L_N) { return create_and_encode(29, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_30_L_N) { return create_and_encode(30, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_31_L_N) { return create_and_encode(31, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_32_L_N) { return create_and_encode(32, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_33_L_N) { return create_and_encode(33, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_34_L_N) { return create_and_encode(34, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_35_L_N) { return create_and_encode(35, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_36_L_N) { return create_and_encode(36, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_37_L_N) { return create_and_encode(37, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_38_L_N) { return create_and_encode(38, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_39_L_N) { return create_and_encode(39, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }
TEST(create_and_encode_40_L_N) { return create_and_encode(40, QR_EC_LEVEL_L, QR_MODE_NUMERIC); }

TEST(create_and_encode_01_M_N) { return create_and_encode( 1, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_02_M_N) { return create_and_encode( 2, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_03_M_N) { return create_and_encode( 3, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_04_M_N) { return create_and_encode( 4, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_05_M_N) { return create_and_encode( 5, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_06_M_N) { return create_and_encode( 6, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_07_M_N) { return create_and_encode( 7, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_08_M_N) { return create_and_encode( 8, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_09_M_N) { return create_and_encode( 9, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_10_M_N) { return create_and_encode(10, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_11_M_N) { return create_and_encode(11, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_12_M_N) { return create_and_encode(12, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_13_M_N) { return create_and_encode(13, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_14_M_N) { return create_and_encode(14, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_15_M_N) { return create_and_encode(15, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_16_M_N) { return create_and_encode(16, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_17_M_N) { return create_and_encode(17, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_18_M_N) { return create_and_encode(18, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_19_M_N) { return create_and_encode(19, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_20_M_N) { return create_and_encode(20, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_21_M_N) { return create_and_encode(21, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_22_M_N) { return create_and_encode(22, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_23_M_N) { return create_and_encode(23, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_24_M_N) { return create_and_encode(24, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_25_M_N) { return create_and_encode(25, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_26_M_N) { return create_and_encode(26, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_27_M_N) { return create_and_encode(27, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_28_M_N) { return create_and_encode(28, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_29_M_N) { return create_and_encode(29, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_30_M_N) { return create_and_encode(30, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_31_M_N) { return create_and_encode(31, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_32_M_N) { return create_and_encode(32, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_33_M_N) { return create_and_encode(33, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_34_M_N) { return create_and_encode(34, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_35_M_N) { return create_and_encode(35, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_36_M_N) { return create_and_encode(36, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_37_M_N) { return create_and_encode(37, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_38_M_N) { return create_and_encode(38, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_39_M_N) { return create_and_encode(39, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }
TEST(create_and_encode_40_M_N) { return create_and_encode(40, QR_EC_LEVEL_M, QR_MODE_NUMERIC); }

TEST(create_and_encode_01_Q_N) { return create_and_encode( 1, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_02_Q_N) { return create_and_encode( 2, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_03_Q_N) { return create_and_encode( 3, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_04_Q_N) { return create_and_encode( 4, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_05_Q_N) { return create_and_encode( 5, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_06_Q_N) { return create_and_encode( 6, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_07_Q_N) { return create_and_encode( 7, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_08_Q_N) { return create_and_encode( 8, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_09_Q_N) { return create_and_encode( 9, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_10_Q_N) { return create_and_encode(10, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_11_Q_N) { return create_and_encode(11, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_12_Q_N) { return create_and_encode(12, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_13_Q_N) { return create_and_encode(13, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_14_Q_N) { return create_and_encode(14, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_15_Q_N) { return create_and_encode(15, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_16_Q_N) { return create_and_encode(16, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_17_Q_N) { return create_and_encode(17, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_18_Q_N) { return create_and_encode(18, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_19_Q_N) { return create_and_encode(19, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_20_Q_N) { return create_and_encode(20, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_21_Q_N) { return create_and_encode(21, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_22_Q_N) { return create_and_encode(22, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_23_Q_N) { return create_and_encode(23, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_24_Q_N) { return create_and_encode(24, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_25_Q_N) { return create_and_encode(25, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_26_Q_N) { return create_and_encode(26, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_27_Q_N) { return create_and_encode(27, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_28_Q_N) { return create_and_encode(28, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_29_Q_N) { return create_and_encode(29, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_30_Q_N) { return create_and_encode(30, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_31_Q_N) { return create_and_encode(31, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_32_Q_N) { return create_and_encode(32, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_33_Q_N) { return create_and_encode(33, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_34_Q_N) { return create_and_encode(34, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_35_Q_N) { return create_and_encode(35, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_36_Q_N) { return create_and_encode(36, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_37_Q_N) { return create_and_encode(37, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_38_Q_N) { return create_and_encode(38, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_39_Q_N) { return create_and_encode(39, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }
TEST(create_and_encode_40_Q_N) { return create_and_encode(40, QR_EC_LEVEL_Q, QR_MODE_NUMERIC); }

TEST(create_and_encode_01_H_N) { return create_and_encode( 1, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_02_H_N) { return create_and_encode( 2, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_03_H_N) { return create_and_encode( 3, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_04_H_N) { return create_and_encode( 4, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_05_H_N) { return create_and_encode( 5, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_06_H_N) { return create_and_encode( 6, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_07_H_N) { return create_and_encode( 7, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_08_H_N) { return create_and_encode( 8, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_09_H_N) { return create_and_encode( 9, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_10_H_N) { return create_and_encode(10, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_11_H_N) { return create_and_encode(11, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_12_H_N) { return create_and_encode(12, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_13_H_N) { return create_and_encode(13, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_14_H_N) { return create_and_encode(14, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_15_H_N) { return create_and_encode(15, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_16_H_N) { return create_and_encode(16, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_17_H_N) { return create_and_encode(17, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_18_H_N) { return create_and_encode(18, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_19_H_N) { return create_and_encode(19, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_20_H_N) { return create_and_encode(20, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_21_H_N) { return create_and_encode(21, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_22_H_N) { return create_and_encode(22, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_23_H_N) { return create_and_encode(23, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_24_H_N) { return create_and_encode(24, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_25_H_N) { return create_and_encode(25, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_26_H_N) { return create_and_encode(26, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_27_H_N) { return create_and_encode(27, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_28_H_N) { return create_and_encode(28, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_29_H_N) { return create_and_encode(29, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_30_H_N) { return create_and_encode(30, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_31_H_N) { return create_and_encode(31, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_32_H_N) { return create_and_encode(32, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_33_H_N) { return create_and_encode(33, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_34_H_N) { return create_and_encode(34, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_35_H_N) { return create_and_encode(35, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_36_H_N) { return create_and_encode(36, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_37_H_N) { return create_and_encode(37, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_38_H_N) { return create_and_encode(38, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_39_H_N) { return create_and_encode(39, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }
TEST(create_and_encode_40_H_N) { return create_and_encode(40, QR_EC_LEVEL_H, QR_MODE_NUMERIC); }


TEST(create_and_encode_01_L_A) { return create_and_encode( 1, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_02_L_A) { return create_and_encode( 2, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_03_L_A) { return create_and_encode( 3, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_04_L_A) { return create_and_encode( 4, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_05_L_A) { return create_and_encode( 5, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_06_L_A) { return create_and_encode( 6, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_07_L_A) { return create_and_encode( 7, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_08_L_A) { return create_and_encode( 8, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_09_L_A) { return create_and_encode( 9, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_10_L_A) { return create_and_encode(10, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_11_L_A) { return create_and_encode(11, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_12_L_A) { return create_and_encode(12, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_13_L_A) { return create_and_encode(13, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_14_L_A) { return create_and_encode(14, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_15_L_A) { return create_and_encode(15, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_16_L_A) { return create_and_encode(16, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_17_L_A) { return create_and_encode(17, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_18_L_A) { return create_and_encode(18, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_19_L_A) { return create_and_encode(19, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_20_L_A) { return create_and_encode(20, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_21_L_A) { return create_and_encode(21, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_22_L_A) { return create_and_encode(22, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_23_L_A) { return create_and_encode(23, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_24_L_A) { return create_and_encode(24, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_25_L_A) { return create_and_encode(25, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_26_L_A) { return create_and_encode(26, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_27_L_A) { return create_and_encode(27, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_28_L_A) { return create_and_encode(28, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_29_L_A) { return create_and_encode(29, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_30_L_A) { return create_and_encode(30, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_31_L_A) { return create_and_encode(31, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_32_L_A) { return create_and_encode(32, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_33_L_A) { return create_and_encode(33, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_34_L_A) { return create_and_encode(34, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_35_L_A) { return create_and_encode(35, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_36_L_A) { return create_and_encode(36, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_37_L_A) { return create_and_encode(37, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_38_L_A) { return create_and_encode(38, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_39_L_A) { return create_and_encode(39, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_40_L_A) { return create_and_encode(40, QR_EC_LEVEL_L, QR_MODE_ALPHANUMERIC); }

TEST(create_and_encode_01_M_A) { return create_and_encode( 1, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_02_M_A) { return create_and_encode( 2, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_03_M_A) { return create_and_encode( 3, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_04_M_A) { return create_and_encode( 4, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_05_M_A) { return create_and_encode( 5, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_06_M_A) { return create_and_encode( 6, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_07_M_A) { return create_and_encode( 7, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_08_M_A) { return create_and_encode( 8, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_09_M_A) { return create_and_encode( 9, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_10_M_A) { return create_and_encode(10, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_11_M_A) { return create_and_encode(11, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_12_M_A) { return create_and_encode(12, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_13_M_A) { return create_and_encode(13, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_14_M_A) { return create_and_encode(14, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_15_M_A) { return create_and_encode(15, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_16_M_A) { return create_and_encode(16, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_17_M_A) { return create_and_encode(17, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_18_M_A) { return create_and_encode(18, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_19_M_A) { return create_and_encode(19, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_20_M_A) { return create_and_encode(20, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_21_M_A) { return create_and_encode(21, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_22_M_A) { return create_and_encode(22, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_23_M_A) { return create_and_encode(23, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_24_M_A) { return create_and_encode(24, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_25_M_A) { return create_and_encode(25, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_26_M_A) { return create_and_encode(26, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_27_M_A) { return create_and_encode(27, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_28_M_A) { return create_and_encode(28, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_29_M_A) { return create_and_encode(29, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_30_M_A) { return create_and_encode(30, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_31_M_A) { return create_and_encode(31, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_32_M_A) { return create_and_encode(32, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_33_M_A) { return create_and_encode(33, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_34_M_A) { return create_and_encode(34, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_35_M_A) { return create_and_encode(35, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_36_M_A) { return create_and_encode(36, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_37_M_A) { return create_and_encode(37, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_38_M_A) { return create_and_encode(38, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_39_M_A) { return create_and_encode(39, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_40_M_A) { return create_and_encode(40, QR_EC_LEVEL_M, QR_MODE_ALPHANUMERIC); }

TEST(create_and_encode_01_Q_A) { return create_and_encode( 1, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_02_Q_A) { return create_and_encode( 2, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_03_Q_A) { return create_and_encode( 3, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_04_Q_A) { return create_and_encode( 4, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_05_Q_A) { return create_and_encode( 5, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_06_Q_A) { return create_and_encode( 6, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_07_Q_A) { return create_and_encode( 7, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_08_Q_A) { return create_and_encode( 8, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_09_Q_A) { return create_and_encode( 9, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_10_Q_A) { return create_and_encode(10, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_11_Q_A) { return create_and_encode(11, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_12_Q_A) { return create_and_encode(12, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_13_Q_A) { return create_and_encode(13, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_14_Q_A) { return create_and_encode(14, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_15_Q_A) { return create_and_encode(15, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_16_Q_A) { return create_and_encode(16, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_17_Q_A) { return create_and_encode(17, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_18_Q_A) { return create_and_encode(18, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_19_Q_A) { return create_and_encode(19, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_20_Q_A) { return create_and_encode(20, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_21_Q_A) { return create_and_encode(21, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_22_Q_A) { return create_and_encode(22, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_23_Q_A) { return create_and_encode(23, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_24_Q_A) { return create_and_encode(24, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_25_Q_A) { return create_and_encode(25, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_26_Q_A) { return create_and_encode(26, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_27_Q_A) { return create_and_encode(27, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_28_Q_A) { return create_and_encode(28, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_29_Q_A) { return create_and_encode(29, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_30_Q_A) { return create_and_encode(30, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_31_Q_A) { return create_and_encode(31, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_32_Q_A) { return create_and_encode(32, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_33_Q_A) { return create_and_encode(33, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_34_Q_A) { return create_and_encode(34, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_35_Q_A) { return create_and_encode(35, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_36_Q_A) { return create_and_encode(36, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_37_Q_A) { return create_and_encode(37, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_38_Q_A) { return create_and_encode(38, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_39_Q_A) { return create_and_encode(39, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_40_Q_A) { return create_and_encode(40, QR_EC_LEVEL_Q, QR_MODE_ALPHANUMERIC); }

TEST(create_and_encode_01_H_A) { return create_and_encode( 1, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_02_H_A) { return create_and_encode( 2, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_03_H_A) { return create_and_encode( 3, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_04_H_A) { return create_and_encode( 4, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_05_H_A) { return create_and_encode( 5, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_06_H_A) { return create_and_encode( 6, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_07_H_A) { return create_and_encode( 7, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_08_H_A) { return create_and_encode( 8, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_09_H_A) { return create_and_encode( 9, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_10_H_A) { return create_and_encode(10, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_11_H_A) { return create_and_encode(11, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_12_H_A) { return create_and_encode(12, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_13_H_A) { return create_and_encode(13, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_14_H_A) { return create_and_encode(14, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_15_H_A) { return create_and_encode(15, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_16_H_A) { return create_and_encode(16, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_17_H_A) { return create_and_encode(17, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_18_H_A) { return create_and_encode(18, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_19_H_A) { return create_and_encode(19, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_20_H_A) { return create_and_encode(20, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_21_H_A) { return create_and_encode(21, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_22_H_A) { return create_and_encode(22, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_23_H_A) { return create_and_encode(23, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_24_H_A) { return create_and_encode(24, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_25_H_A) { return create_and_encode(25, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_26_H_A) { return create_and_encode(26, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_27_H_A) { return create_and_encode(27, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_28_H_A) { return create_and_encode(28, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_29_H_A) { return create_and_encode(29, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_30_H_A) { return create_and_encode(30, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_31_H_A) { return create_and_encode(31, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_32_H_A) { return create_and_encode(32, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_33_H_A) { return create_and_encode(33, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_34_H_A) { return create_and_encode(34, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_35_H_A) { return create_and_encode(35, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_36_H_A) { return create_and_encode(36, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_37_H_A) { return create_and_encode(37, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_38_H_A) { return create_and_encode(38, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_39_H_A) { return create_and_encode(39, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }
TEST(create_and_encode_40_H_A) { return create_and_encode(40, QR_EC_LEVEL_H, QR_MODE_ALPHANUMERIC); }


TEST(create_and_encode_01_L_B) { return create_and_encode( 1, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_02_L_B) { return create_and_encode( 2, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_03_L_B) { return create_and_encode( 3, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_04_L_B) { return create_and_encode( 4, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_05_L_B) { return create_and_encode( 5, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_06_L_B) { return create_and_encode( 6, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_07_L_B) { return create_and_encode( 7, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_08_L_B) { return create_and_encode( 8, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_09_L_B) { return create_and_encode( 9, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_10_L_B) { return create_and_encode(10, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_11_L_B) { return create_and_encode(11, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_12_L_B) { return create_and_encode(12, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_13_L_B) { return create_and_encode(13, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_14_L_B) { return create_and_encode(14, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_15_L_B) { return create_and_encode(15, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_16_L_B) { return create_and_encode(16, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_17_L_B) { return create_and_encode(17, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_18_L_B) { return create_and_encode(18, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_19_L_B) { return create_and_encode(19, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_20_L_B) { return create_and_encode(20, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_21_L_B) { return create_and_encode(21, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_22_L_B) { return create_and_encode(22, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_23_L_B) { return create_and_encode(23, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_24_L_B) { return create_and_encode(24, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_25_L_B) { return create_and_encode(25, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_26_L_B) { return create_and_encode(26, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_27_L_B) { return create_and_encode(27, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_28_L_B) { return create_and_encode(28, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_29_L_B) { return create_and_encode(29, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_30_L_B) { return create_and_encode(30, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_31_L_B) { return create_and_encode(31, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_32_L_B) { return create_and_encode(32, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_33_L_B) { return create_and_encode(33, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_34_L_B) { return create_and_encode(34, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_35_L_B) { return create_and_encode(35, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_36_L_B) { return create_and_encode(36, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_37_L_B) { return create_and_encode(37, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_38_L_B) { return create_and_encode(38, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_39_L_B) { return create_and_encode(39, QR_EC_LEVEL_L, QR_MODE_BYTE); }
TEST(create_and_encode_40_L_B) { return create_and_encode(40, QR_EC_LEVEL_L, QR_MODE_BYTE); }

TEST(create_and_encode_01_M_B) { return create_and_encode( 1, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_02_M_B) { return create_and_encode( 2, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_03_M_B) { return create_and_encode( 3, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_04_M_B) { return create_and_encode( 4, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_05_M_B) { return create_and_encode( 5, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_06_M_B) { return create_and_encode( 6, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_07_M_B) { return create_and_encode( 7, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_08_M_B) { return create_and_encode( 8, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_09_M_B) { return create_and_encode( 9, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_10_M_B) { return create_and_encode(10, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_11_M_B) { return create_and_encode(11, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_12_M_B) { return create_and_encode(12, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_13_M_B) { return create_and_encode(13, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_14_M_B) { return create_and_encode(14, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_15_M_B) { return create_and_encode(15, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_16_M_B) { return create_and_encode(16, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_17_M_B) { return create_and_encode(17, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_18_M_B) { return create_and_encode(18, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_19_M_B) { return create_and_encode(19, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_20_M_B) { return create_and_encode(20, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_21_M_B) { return create_and_encode(21, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_22_M_B) { return create_and_encode(22, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_23_M_B) { return create_and_encode(23, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_24_M_B) { return create_and_encode(24, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_25_M_B) { return create_and_encode(25, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_26_M_B) { return create_and_encode(26, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_27_M_B) { return create_and_encode(27, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_28_M_B) { return create_and_encode(28, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_29_M_B) { return create_and_encode(29, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_30_M_B) { return create_and_encode(30, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_31_M_B) { return create_and_encode(31, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_32_M_B) { return create_and_encode(32, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_33_M_B) { return create_and_encode(33, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_34_M_B) { return create_and_encode(34, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_35_M_B) { return create_and_encode(35, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_36_M_B) { return create_and_encode(36, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_37_M_B) { return create_and_encode(37, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_38_M_B) { return create_and_encode(38, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_39_M_B) { return create_and_encode(39, QR_EC_LEVEL_M, QR_MODE_BYTE); }
TEST(create_and_encode_40_M_B) { return create_and_encode(40, QR_EC_LEVEL_M, QR_MODE_BYTE); }

TEST(create_and_encode_01_Q_B) { return create_and_encode( 1, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_02_Q_B) { return create_and_encode( 2, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_03_Q_B) { return create_and_encode( 3, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_04_Q_B) { return create_and_encode( 4, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_05_Q_B) { return create_and_encode( 5, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_06_Q_B) { return create_and_encode( 6, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_07_Q_B) { return create_and_encode( 7, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_08_Q_B) { return create_and_encode( 8, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_09_Q_B) { return create_and_encode( 9, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_10_Q_B) { return create_and_encode(10, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_11_Q_B) { return create_and_encode(11, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_12_Q_B) { return create_and_encode(12, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_13_Q_B) { return create_and_encode(13, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_14_Q_B) { return create_and_encode(14, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_15_Q_B) { return create_and_encode(15, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_16_Q_B) { return create_and_encode(16, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_17_Q_B) { return create_and_encode(17, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_18_Q_B) { return create_and_encode(18, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_19_Q_B) { return create_and_encode(19, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_20_Q_B) { return create_and_encode(20, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_21_Q_B) { return create_and_encode(21, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_22_Q_B) { return create_and_encode(22, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_23_Q_B) { return create_and_encode(23, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_24_Q_B) { return create_and_encode(24, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_25_Q_B) { return create_and_encode(25, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_26_Q_B) { return create_and_encode(26, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_27_Q_B) { return create_and_encode(27, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_28_Q_B) { return create_and_encode(28, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_29_Q_B) { return create_and_encode(29, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_30_Q_B) { return create_and_encode(30, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_31_Q_B) { return create_and_encode(31, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_32_Q_B) { return create_and_encode(32, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_33_Q_B) { return create_and_encode(33, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_34_Q_B) { return create_and_encode(34, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_35_Q_B) { return create_and_encode(35, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_36_Q_B) { return create_and_encode(36, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_37_Q_B) { return create_and_encode(37, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_38_Q_B) { return create_and_encode(38, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_39_Q_B) { return create_and_encode(39, QR_EC_LEVEL_Q, QR_MODE_BYTE); }
TEST(create_and_encode_40_Q_B) { return create_and_encode(40, QR_EC_LEVEL_Q, QR_MODE_BYTE); }

TEST(create_and_encode_01_H_B) { return create_and_encode( 1, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_02_H_B) { return create_and_encode( 2, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_03_H_B) { return create_and_encode( 3, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_04_H_B) { return create_and_encode( 4, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_05_H_B) { return create_and_encode( 5, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_06_H_B) { return create_and_encode( 6, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_07_H_B) { return create_and_encode( 7, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_08_H_B) { return create_and_encode( 8, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_09_H_B) { return create_and_encode( 9, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_10_H_B) { return create_and_encode(10, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_11_H_B) { return create_and_encode(11, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_12_H_B) { return create_and_encode(12, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_13_H_B) { return create_and_encode(13, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_14_H_B) { return create_and_encode(14, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_15_H_B) { return create_and_encode(15, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_16_H_B) { return create_and_encode(16, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_17_H_B) { return create_and_encode(17, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_18_H_B) { return create_and_encode(18, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_19_H_B) { return create_and_encode(19, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_20_H_B) { return create_and_encode(20, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_21_H_B) { return create_and_encode(21, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_22_H_B) { return create_and_encode(22, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_23_H_B) { return create_and_encode(23, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_24_H_B) { return create_and_encode(24, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_25_H_B) { return create_and_encode(25, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_26_H_B) { return create_and_encode(26, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_27_H_B) { return create_and_encode(27, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_28_H_B) { return create_and_encode(28, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_29_H_B) { return create_and_encode(29, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_30_H_B) { return create_and_encode(30, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_31_H_B) { return create_and_encode(31, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_32_H_B) { return create_and_encode(32, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_33_H_B) { return create_and_encode(33, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_34_H_B) { return create_and_encode(34, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_35_H_B) { return create_and_encode(35, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_36_H_B) { return create_and_encode(36, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_37_H_B) { return create_and_encode(37, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_38_H_B) { return create_and_encode(38, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_39_H_B) { return create_and_encode(39, QR_EC_LEVEL_H, QR_MODE_BYTE); }
TEST(create_and_encode_40_H_B) { return create_and_encode(40, QR_EC_LEVEL_H, QR_MODE_BYTE); }

/**
 * @brief Test qr_min_version returns 0 for too large messages
 */
TEST(too_large_message)
{
	unsigned version;
	qr_ecl level;

	// Test with a message that's too large for any version
	const char *large_message = "This is a very long message that exceeds the maximum capacity of any QR code version. "
	                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
	                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
	                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris. "
	                            "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore. "
	                            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia. "
	                            "Sed ut perspiciatis unde omnis iste natus error sit voluptatem. "
	                            "Nemo enim ipsam voluptatem quia voluptas sit aspernatur. "
	                            "Aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione. "
	                            "Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet. "
	                            "Consectetur, adipisci velit, sed quia non numquam eius modi tempora. "
	                            "Incidunt ut labore et dolore magnam aliquam quaerat voluptatem. "
	                            "Ut enim ad minima veniam, quis nostrum exercitationem ullam. "
	                            "Corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur. "
	                            "Quis autem vel eum iure reprehenderit qui in ea voluptate velit. "
	                            "Esse quam nihil molestiae consequatur, vel illum qui dolorem. "
	                            "Eum fugiat quo voluptas nulla pariatur et iusto odio dignissimos. "
	                            "Ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti. "
	                            "Quos dolores et quas molestias excepturi sint occaecati cupiditate. "
	                            "Non provident, similique sunt in culpa qui officia deserunt. "
	                            "Mollitia animi, id est laborum et dolorum fuga. "
	                            "Et harum quidem rerum facilis est et expedita distinctio. "
	                            "Nam libero tempore, cum soluta nobis est eligendi optio. "
	                            "Cumque nihil impedit quo minus id quod maxime placeat. "
	                            "Facere possimus, omnis voluptas assumenda est, omnis dolor repellendus. "
	                            "Temporibus autem quibusdam et aut officiis debitis aut rerum. "
	                            "Necessitatibus saepe eveniet ut et voluptates repudiandae sint. "
	                            "Et molestiae non recusandae itaque earum rerum hic tenetur. "
	                            "Sapiente delectus reiciendis voluptatibus maiores alias consequatur. "
	                            "Perferendis doloribus asperiores repellat extra long message test. "
	                            "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium. "
	                            "Nam libero tempore, cum soluta nobis est eligendi optio cumque nihil impedit. "
	                            "Itaque earum rerum hic tenetur a sapiente delectus ut aut reiciendis voluptatibus. "
	                            "Maiores alias consequatur aut perferendis doloribus asperiores repellat. "
	                            "Et harum quidem rerum facilis est et expedita distinctio nam libero tempore. "
	                            "Cum soluta nobis est eligendi optio cumque nihil impedit quo minus id quod. "
	                            "Maxime placeat facere possimus omnis voluptas assumenda est omnis dolor. "
	                            "Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus. "
	                            "Saepe eveniet ut et voluptates repudiandae sint et molestiae non recusandae. "
	                            "Itaque earum rerum hic tenetur a sapiente delectus ut aut reiciendis. "
	                            "Voluptatibus maiores alias consequatur aut perferendis doloribus asperiores. "
	                            "Ut enim ad minima veniam quis nostrum exercitationem ullam corporis suscipit. "
	                            "Laboriosam nisi ut aliquid ex ea commodi consequatur quis autem vel. "
	                            "Eum iure reprehenderit qui in ea voluptate velit esse quam nihil. "
	                            "Molestiae consequatur vel illum qui dolorem eum fugiat quo voluptas. "
	                            "Nulla pariatur et iusto odio dignissimos ducimus qui blanditiis praesentium. "
	                            "Voluptatum deleniti atque corrupti quos dolores et quas molestias. "
	                            "Excepturi sint occaecati cupiditate non provident similique sunt in culpa. "
	                            "Qui officia deserunt mollitia animi id est laborum et dolorum fuga. "
	                            "Et harum quidem rerum facilis est et expedita distinctio nam libero. "
	                            "Tempore cum soluta nobis est eligendi optio cumque nihil impedit. "
	                            "Quo minus id quod maxime placeat facere possimus omnis voluptas. "
	                            "Assumenda est omnis dolor repellendus temporibus autem quibusdam et. "
	                            "Aut officiis debitis aut rerum necessitatibus saepe eveniet ut. "
	                            "Et voluptates repudiandae sint et molestiae non recusandae itaque. "
	                            "Earum rerum hic tenetur a sapiente delectus ut aut reiciendis. "
	                            "Voluptatibus maiores alias consequatur aut perferendis doloribus asperiores.";

	// Test for each error correction level
	for (level = 0; level < QR_EC_LEVEL_COUNT; ++level)
	{
		version = qr_min_version(QR_MODE_BYTE, level, strlen(large_message));
		test_eq(version, 0, "Too large message should return 0");
	}

	return TEST_SUCCESS;
}
