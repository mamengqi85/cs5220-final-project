CC=gcc
MPICC=mpicc
CFLAGS=-Wall -g -std=gnu99 -O3

.PHONY: exe test_u clean realclean


# === Executables

test_u: test_utility.x

test_utility.x: test_utility.c utility.o params.o
	$(CC) $(CFLAGS) $^ -o $@

params.o: params.c params.h
	$(CC) -c $(CFLAGS) $< 

record.o: record.c record.h
	$(CC) -c $(CFLAGS) $< 

utility.o: utility.c utility.h
	$(CC) -c $(CFLAGS) $< 

%.o: %.c
	$(CC) -c $(CFLAGS) $< 

# === Cleanup and tarball

clean:
	rm -f *.o 

realclean: clean
	rm -f test_u.x

