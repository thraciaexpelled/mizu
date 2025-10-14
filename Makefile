CC_FLAGS=-Wall -Wextra -Wpedantic -Iinclude/ -O0 -ggdb
OBJ=main.o carrier.o project.o config.o

.PHONY: clean install

mizu: $(OBJ)
	$(CC) $(CC_FLAGS) -o $@ $(OBJ)

%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm -f ./mizu
	rm -f *.o

install:
	install -m775 mizu /usr/local/bin/mizu
