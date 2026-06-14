#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test/base.h>

typedef struct test_node
{
	const char *name;
	bool is_preparation;
	struct test_result (*fn)(void);
	struct test_result res;
	struct test_node *next;
} test_node;

typedef struct group_node
{
	const char *name;
	test_node *tests;
	size_t test_count;
	size_t longest_test_name;
	struct group_node *next;
} group_node;

static group_node *group_head = nullptr;
static size_t group_count = 0;
static size_t longest_group_name = 0;

test_node *
append_test(test_node **head, const char *name, struct test_result (*fn)(void))
{
	test_node *new_test, *current;

	new_test = test_malloc(sizeof(test_node));
	new_test->name = name;
	new_test->is_preparation = false;
	new_test->fn = fn;
	new_test->res = TEST_SUCCESS;
	new_test->next = nullptr;

	if (!*head)
	{
		*head = new_test;
	}
	else
	{
		for (current = *head; current->next; current = current->next);
		current->next = new_test;
	}

	return new_test;
}

test_node *
prepend_preparation(test_node **head, const char *name, struct test_result (*fn)(void))
{
	test_node *new_test;

	new_test = test_malloc(sizeof(test_node));
	new_test->name = name;
	new_test->is_preparation = true;
	new_test->fn = fn;
	new_test->res = TEST_SUCCESS;
	new_test->next = *head;

	*head = new_test;

	return new_test;
}

group_node *
append_group(group_node **head, const char *name)
{
	group_node *current;

	group_node *new_group = test_malloc(sizeof(group_node));
	new_group->name = name;
	new_group->tests = nullptr;
	new_group->test_count = 0;
	new_group->longest_test_name = 0;
	new_group->next = nullptr;

	if (!*head)
	{
		*head = new_group;
	}
	else
	{
		for (current = *head; current->next; current = current->next);
		current->next = new_group;
	}

	if (strlen(new_group->name) > longest_group_name)
		longest_group_name = strlen(new_group->name);
	++group_count;

	return new_group;
}

group_node *
group_exists(group_node **head, const char *name)
{
	group_node *current;

	for (current = *head; current; current = current->next)
		if (!strcmp(current->name, name))
			return current;

	return nullptr;
}

void
test_register_case(const char *group_name, const char *test_name, struct test_result (*fn)(void))
{
	group_node *group;
	test_node *test;

	if (!(group = group_exists(&group_head, group_name)))
		group = append_group(&group_head, group_name);

	test = append_test(&group->tests, test_name, fn);
	if (strlen(test->name) > group->longest_test_name)
		group->longest_test_name = strlen(test->name);

	++group->test_count;
}

void
test_register_setup(const char *group_name, const char *test_name, struct test_result (*fn)(void))
{
	group_node *group;

	if (!(group = group_exists(&group_head, group_name)))
		group = append_group(&group_head, group_name);

	prepend_preparation(&group->tests, test_name, fn);
}

typedef struct memory_node
{
	void *ptr;
	struct memory_node *next;
} memory_node;

memory_node *memory_head = nullptr;

void *
test_malloc(size_t size)
{
	void *ptr = malloc(size);
	memory_node *new_memory = malloc(sizeof(memory_node));
	new_memory->ptr = ptr;
	new_memory->next = memory_head;
	memory_head = new_memory;
	return ptr;
}

void
test_free(void)
{
	memory_node *current, *next;

	for (current = memory_head; current; current = next)
	{
		free(current->ptr);
		next = current->next;
		free(current);
	}

	memory_head = nullptr;
}

constexpr size_t BAR_WIDTH = 40;

void
print_group_progress(group_node *group, size_t ran, size_t failures)
{
	size_t i;

	printf("> Test group %s%-*s %3zu test(s) ran; %3zu failed. ", group->name, (int) (longest_group_name - strlen(group->name) + 1), ":", ran, failures);
	printf("[\x1b[32m\x1b[7m");
	for (i = 0; i < (unsigned) (((ran - failures) / (float) group->test_count) * BAR_WIDTH); ++i)
		printf("-");
	printf("\x1b[31m");
	for (; i < BAR_WIDTH; ++i)
		printf("-");
	printf("\x1b[27m\x1b[0m]\n");
}

size_t
run_test_group(group_node *group)
{
	test_node *current_test;
	size_t ran = 0, failures = 0;

	print_group_progress(group, ran, failures);

	for (current_test = group->tests; current_test; current_test = current_test->next)
	{
		if (current_test->is_preparation)
		{
			current_test->res = current_test->fn();
			if (current_test->res.failed)
			{
				group->test_count = 0;
				break;
			}
			continue;
		}

		printf(" => %-*s (%2zu/%2zu)", (int) group->longest_test_name, current_test->name, ran + 1, group->test_count);
		fflush(stdout);
		current_test->res = current_test->fn();
		++ran;
		if (current_test->res.failed) ++failures;
		printf("\r\x1b[K\x1b[A");
		print_group_progress(group, ran, failures);
	}

	return failures;
}

int
main(void)
{
	group_node *current_group;
	test_node *current_test;
	size_t total_failures = 0, total_tests = 0;

	printf("Running test suite [ %zu test group(s) ]:\n\n", group_count);

	for (current_group = group_head; current_group; current_group = current_group->next)
	{
		total_failures += run_test_group(current_group);
		total_tests += current_group->test_count;
	}

	printf("\nSummary: %zu test(s) ran; %zu failed.\n", total_tests, total_failures);
	for (current_group = group_head; current_group; current_group = current_group->next)
	{
		for (current_test = current_group->tests; current_test; current_test = current_test->next)
		{
			if (!current_test->res.failed) continue;
			printf("  => %s:%zu [%s] failed:\n    => %s\n", current_group->name, current_test->res.line, current_test->name, current_test->res.message);
		}
	}

	test_free();
	return total_failures;
}
