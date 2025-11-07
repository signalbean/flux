CFLAGS = -Wall -Wextra -std=c99 -O2
RELEASE_FLAGS = -Os -s -DNDEBUG -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections
SOURCES = main.c args.c flux.c utils.c
OBJECTS = $(SOURCES:%.c=obj/%.o)
TARGET = bin/flux

all: $(TARGET)

$(TARGET): $(OBJECTS) | bin
	gcc $(OBJECTS) -o $(TARGET) -lcurl

obj/%.o: %.c | obj
	gcc $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

bin:
	mkdir -p bin

release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

clean:
	rm -rf obj bin

.PHONY: all release clean
