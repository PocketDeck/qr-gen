#ifndef TEST_BASE_H
#define TEST_BASE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_result
{
	int failed;
	const char *message;
	size_t line;
};
void test_register(const char *group, const char *name, struct test_result (*fn)(void));
void before_register(const char *group, const char *name, struct test_result (*fn)(void));

#define TEST(test_id) \
static struct test_result __test_## test_id(void); \
__attribute__((constructor)) static void __testregister_## test_id(void) \
{ \
	test_register(__FILE__, #test_id, __test_## test_id); \
} \
static struct test_result __test_## test_id(void)

#define BEFORE() \
static struct test_result __before_all(void); \
__attribute__((constructor)) static void __beforeregister(void) \
{ \
	before_register(__FILE__, "BEFORE", __before_all); \
} \
static struct test_result __before_all(void)

#define INT_MAX_CHARS 11

#define TEST_SUCCESS (struct test_result){0, NULL, 0}
#define TEST_FAILURE(message) (struct test_result){1, message, __LINE__}

#define assert_base(actual, expected, message, operator) \
	do { \
		int __assert_actual = (actual); \
		int __assert_expected = (expected); \
		if (!(__assert_actual operator __assert_expected)) \
		{ \
			size_t __assert_length = \
				strlen(message) + \
				strlen(": actual {%d} " #operator " expected {%d}") + (2 * INT_MAX_CHARS) + 1; \
			char *__assert_message = malloc(__assert_length); \
			if (__assert_message == NULL) return (struct test_result){1, "assert: malloc failed", __LINE__}; \
			snprintf(__assert_message, __assert_length, "%s: actual {%d} " #operator " expected {%d}", message, __assert_actual, __assert_expected); \
			return (struct test_result){1, __assert_message, __LINE__}; \
		} \
	} while (0)

#define assert_equal(actual, expected, message) assert_base(actual, expected, message, ==)
#define assert_not_equal(actual, expected, message) assert_base(actual, expected, message, !=)
#define assert_less_than(actual, expected, message) assert_base(actual, expected, message, <)
#define assert_greater_than(actual, expected, message) assert_base(actual, expected, message, >)
#define assert_less_than_or_equal(actual, expected, message) assert_base(actual, expected, message, <=)
#define assert_greater_than_or_equal(actual, expected, message) assert_base(actual, expected, message, >=)

#endif // TEST_BASE_H
