CFLAGS = -Wall -Wextra -std=c99 -O2
RELEASE_FLAGS = -Os -s -DNDEBUG -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections
SOURCES = main.c args.c flux.c utils.c
OBJECTS = $(SOURCES:.c=.o)

all: flux

flux: $(OBJECTS)
	gcc $(OBJECTS) -o flux -lcurl

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

release: CFLAGS += $(RELEASE_FLAGS)
release: clean flux

clean:
	rm -f $(OBJECTS) flux

.PHONY: all release clean
