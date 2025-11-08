CFLAGS = -Wall -Wextra -std=c99 -O2 -Iinclude
RELEASE_FLAGS = -Os -s -DNDEBUG -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,obj/%.o,$(SOURCES))
TARGET = bin/flux

all: $(TARGET)

$(TARGET): $(OBJECTS) | bin
	gcc $(OBJECTS) -o $(TARGET) -lcurl $(LDFLAGS)

obj/%.o: src/%.c | obj
	gcc $(CFLAGS) -c $< -o $@

obj:
	mkdir -p $@

bin:
	mkdir -p $@

release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

clean:
	rm -rf obj bin

.PHONY: all release clean
