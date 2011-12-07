CC=gcc
MPICC=mpicc
CFLAGS=-Wall -g -std=gnu99 -O3

.PHONY: exe clean realclean


# === Executables

exe: test_utility.x try_gen_operations.x ga.x

ga.x: ga_main.c ga.o params.o record.o utility.o genetic_operations.o
	$(MPICC) $(CFLAGS) $^ -o $@

test_utility.x: test_utility.c utility.o params.o
	$(CC) $(CFLAGS) $^ -o $@

try_gen_operations.x: try_gen_operations.c utility.o params.o record.o genetic_operations.o
	$(CC) $(CFLAGS) $^ -o $@

ga.o: ga.c ga.h params.h record.h utility.h genetic_operations.h
	$(MPICC) $(CFLAGS) $^ -c $@

params.o: params.c params.h
	$(CC) -c $(CFLAGS) $< 

record.o: record.c record.h params.h utility.h
	$(CC) -c $(CFLAGS) $< 

utility.o: utility.c utility.h params.h
	$(CC) -c $(CFLAGS) $< 

genetic_operations.o: genetic_operations.c genetic_operations.h params.h utility.h
	$(CC) -c $(CFLAGS) $< 

%.o: %.c
	$(CC) -c $(CFLAGS) $< 

# === Cleanup and tarball

clean:
	rm -f *.o 

realclean: clean
	rm -f *.x

clean_all:
	rm -f *.qsub.o*
	rm -f *.qsub.po*
	rm -f *.o 
	rm -f *.x
	rm -f main.aux main.log main.out

run: ga.x
	rm -f *.qsub.o*
	rm -f *.qsub.po*
	rm -f core.*
	qsub run-global.qsub
