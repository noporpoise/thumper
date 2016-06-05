

all: thumper

thumper: thumper.c
	$(CC) -Wall -Wextra -O2 -o $@ $<

clean:
	rm -rf thumper test.thumper.txt

test: thumper
	./thumper -n 10000 test.thumper.txt
	./checker test.thumper.txt
	rm -rf test.thumper.txt

.PHONY: all clean test
