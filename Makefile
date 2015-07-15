

all: thumper

thumper: thumper.c
	$(CC) -Wall -Wextra -o $@ $<

clean:
	rm -rf thumper test.thumper.txt

test: thumper
	./thumper test.thumper.txt
	./checker test.thumper.txt

.PHONY: all clean test
