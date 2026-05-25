#ifndef TEST_BASE_H
#define TEST_BASE_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct test_result
{
	bool failed;
	const char *message;
	size_t line;
};
void test_register_case(const char *group, const char *name, struct test_result (*fn)(void));
void test_register_setup(const char *group, const char *name, struct test_result (*fn)(void));

#define TEST(test_id) \
static struct test_result test_case_##test_id(void); \
__attribute__((constructor)) static void test_register_case_##test_id(void) \
{ \
	test_register_case(__FILE__, #test_id, test_case_##test_id); \
} \
static struct test_result test_case_##test_id(void)

#define PASTE(a, b) a##b
#define EXPAND_AND_PASTE(a, b) PASTE(a, b)

#define BEFORE() \
static struct test_result EXPAND_AND_PASTE(test_before_, __LINE__)(void); \
__attribute__((constructor)) static void EXPAND_AND_PASTE(test_register_setup_, __LINE__)(void) \
{ \
	test_register_setup(__FILE__, "BEFORE", EXPAND_AND_PASTE(test_before_, __LINE__)); \
} \
static struct test_result EXPAND_AND_PASTE(test_before_, __LINE__)(void)

void *test_malloc(size_t size);

#define INT_MAX_CHARS 11

#define TEST_SUCCESS (struct test_result) { false, nullptr, __LINE__ }
#define TEST_FAILURE(message) (struct test_result) { true, (message), __LINE__ }

#define test_base(lhs, rhs, message, operator) \
	do { \
		int _test_lhs = (lhs); \
		int _test_rhs = (rhs); \
		if (!(_test_lhs operator _test_rhs)) \
		{ \
			size_t _test_length = \
				strlen(message) + \
				strlen(":  " #operator " ") + \
				(2 * INT_MAX_CHARS) + 1; \
			char *_test_message = test_malloc(_test_length); \
			if (_test_message == nullptr) return TEST_FAILURE("test: test_malloc failed"); \
			snprintf(_test_message, _test_length, "%s: %d " #operator " %d", message, _test_lhs, _test_rhs); \
			return TEST_FAILURE(_test_message); \
		} \
	} while (0)

#define test_eq(lhs, rhs, message) test_base(lhs, rhs, message, ==)
#define test_ne(lhs, rhs, message) test_base(lhs, rhs, message, !=)
#define test_lt(lhs, rhs, message) test_base(lhs, rhs, message, <)
#define test_gt(lhs, rhs, message) test_base(lhs, rhs, message, >)
#define test_le(lhs, rhs, message) test_base(lhs, rhs, message, <=)
#define test_ge(lhs, rhs, message) test_base(lhs, rhs, message, >=)

#endif // TEST_BASE_H
