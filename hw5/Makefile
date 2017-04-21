## Makefile

CC     = gcc
# CFLAGS = -g3 -std=gnu99 -pedantic -Wall
CFLAGS = -std=gnu99 -pedantic -Wall -g3
HWK    = /c/cs223/hw5

Cloud:  Cloud.o btree.o avltree.o
	${CC} ${CFLAGS} -o Cloud Cloud.o btree.o avltree.o

Cloud.o: Cloud.c
	${CC} ${CFLAGS} -c Cloud.c

## add stuff for btree's

btreetest: btreetest.o btree.o
	${CC} ${CFLAGS} -o btreetest btreetest.o btree.o

btreetest.o: btreetest.c /c/cs223/hw5/btree.h 
	${CC} ${CFLAGS} -c btreetest.c

btreetestargs: btreetestargs.o btree.o
	${CC} ${CFLAGS} -o btreetestargs btreetestargs.o btree.o

btreetestargs.o: btreetestargs.c /c/cs223/hw5/btree.h 
	${CC} ${CFLAGS} -c btreetestargs.c

btree.o: btree.c
	${CC} ${CFLAGS} -c btree.c

avltree.o: avltree.c
	${CC} ${CFLAGS} -c avltree.c

clean:
	rm -i *.o *~

