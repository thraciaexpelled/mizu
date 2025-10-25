CFLAGS:=-Wall -Wextra -Wpedantic -Iinclude/ -O0 -ggdb $(shell pkg-config --cflags jansson)
LDFLAGS:=$(shell pkg-config --libs jansson)
OBJ:=main.o carrier.o project.o config.o

.PHONY: clean install

mizu: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

log.o: src/log.c
	$(CC) $(CFLAGS) -DLOG_USE_COLOR -c -o $@ $<

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f ./mizu
	rm -f *.o

install:
	install -m775 mizu /usr/local/bin/mizu
