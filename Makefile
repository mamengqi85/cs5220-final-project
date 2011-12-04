CC=gcc
MPICC=mpicc
CFLAGS=-Wall -g -std=gnu99 -O3

.PHONY: exe clean realclean


# === Executables

exe: test_utility.x ga.x

ga.x: ga_main.c ga.o params.o record.o utility.o
	$(CC) $(CFLAGS) $^ -o $@

test_utility.x: test_utility.c utility.o params.o
	$(CC) $(CFLAGS) $^ -o $@

ga.o: ga.c ga.h genetic_operations.h params.h record.h utility.h
	$(CC) $(CFLAGS) $^ -c $@

params.o: params.c params.h
	$(CC) -c $(CFLAGS) $< 

record.o: record.c record.h params.h
	$(CC) -c $(CFLAGS) $< 

utility.o: utility.c utility.h params.h
	$(CC) -c $(CFLAGS) $< 

%.o: %.c
	$(CC) -c $(CFLAGS) $< 

# === Cleanup and tarball

clean:
	rm -f *.o 

realclean: clean
	rm -f *.x

