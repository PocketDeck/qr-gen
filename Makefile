SRC_DIR           := qr
TEST_DIR          := test
BUILD_DIR         := build
RELEASE_BUILD_DIR := $(BUILD_DIR)/release
TEST_BUILD_DIR    := $(BUILD_DIR)/test
TARGET_RELEASE    := $(RELEASE_BUILD_DIR)/qr-gen
TARGET_TEST       := $(TEST_BUILD_DIR)/qr-test

SRCS  := $(wildcard $(SRC_DIR)/*.c)
OBJS  := $(patsubst $(SRC_DIR)/%.c, $(RELEASE_BUILD_DIR)/%.o, $(SRCS))
TESTS := $(wildcard $(TEST_DIR)/*.c)
TOBJS := $(patsubst $(TEST_DIR)/%.c, $(TEST_BUILD_DIR)/%.o, $(TESTS))
DEPS  := $(OBJS:.o=.d) $(TOBJS:.o=.d)

CFLAGS := -Wall -Wextra -Wpedantic -Werror -I. -MMD -MP -O3 -flto=auto
TESTFLAGS := -Wl,--allow-multiple-definition

all: $(TARGET_RELEASE) $(TARGET_TEST)

$(TARGET_RELEASE): $(OBJS) | $(RELEASE_BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_TEST): $(filter-out $(RELEASE_BUILD_DIR)/main.o,$(OBJS)) $(TOBJS) | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(TESTFLAGS) -o $@ $^

$(RELEASE_BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(RELEASE_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(RELEASE_BUILD_DIR) $(TEST_BUILD_DIR):
	mkdir -p $@

.PHONY: all clean run test

clean:
	rm -rf $(BUILD_DIR) $(RELEASE_BUILD_DIR) $(TEST_BUILD_DIR)

run: $(TARGET_RELEASE)
	./$(TARGET_RELEASE) "$(TEXT)" "$(ECL)" "$(OUTFMT)"

test: $(TARGET_TEST)
	./$(TARGET_TEST)

-include $(DEPS)
