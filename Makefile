CC = gcc
AR = ar
ARFLAGS = rcs
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

CFLAGS = -Wall -Wextra -O2 -MMD -MP
DEBUG ?= 0
ifeq ($(DEBUG),1)
	CFLAGS += -g -O0
endif

EXAMPLE = examples/basic.c

LIBNAME = libds.a
LIBRARY = $(BUILD_DIR)/$(LIBNAME)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

DEPS = $(OBJS:.o=.d)

all: $(LIBRARY)

DESTDIR ?=
PREFIX ?= /usr/local
INSTALLED_HEADERS = $(addprefix $(DESTDIR)$(PREFIX)/include/, $(notdir $(HEADERS)))

install: $(LIBRARY) $(HEADERS)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp $(LIBRARY) $(DESTDIR)$(PREFIX)/lib/$(LIBNAME)
	cp $(INCLUDE_DIR)/*.h $(DESTDIR)$(PREFIX)/include

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(LIBNAME)
	rm -f $(INSTALLED_HEADERS)

$(LIBRARY): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

example: $(LIBRARY)
	$(CC) $(CFLAGS) $(EXAMPLE) -I$(INCLUDE_DIR) $(LIBRARY) -o $(BUILD_DIR)/example

run-example: example
	$(BUILD_DIR)/example

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install uninstall clean example run-example

-include $(DEPS)