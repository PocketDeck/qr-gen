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
TEST(all_versions)
{
	unsigned version;
	qr_ecl level;
	qr_mode mode;
	const char *test_text = "123456";

	for (version = 1; version <= QR_VERSION_COUNT; ++version)
	{
		for (level = 0; level < QR_EC_LEVEL_COUNT; ++level)
		{
			for (mode = 0; mode < QR_MODE_COUNT; ++mode)
			{
				qr_code *qr = qr_create(version, mode, level);
				if (!qr) return TEST_FAILURE("Failed to create QR code");

				qr_encode_text(qr, test_text);
				qr_destroy(qr);
			}
		}
	}

	return TEST_SUCCESS;
}

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
