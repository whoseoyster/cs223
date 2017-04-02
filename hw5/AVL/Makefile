CC=gcc
CFLAGS=-std=c99 -g3 -Wall -pedantic

all: treeTest speedTest

test: all
	valgrind -q ./treeTest
	time ./speedTest 1000000

treeTest: avltree.c avltree.h
	$(CC) $(CFLAGS) -DTEST_MAIN -DPRINT_AFTER_OPERATIONS -o $@ $<

speedTest: speedTest.o tree.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o treeTest speedTest