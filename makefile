CC = gcc
FLGS_OBJ = -c -o
FLGS_PROG = -o
LIBS = -lz -pthread
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS := $(filter-out Prog.o,$(OBJS))
PROG := Prog.out

all: CLEAN BUILD RUN

BUILD: START $(OBJS) $(PROG) END

%.o: %.c
	$(CC) $(FLGS_OBJ) $@ $^ $(LIBS)
$(PROG): Prog.c
	$(CC) $(FLGS_PROG) $@ $^ $(OBJS) $(LIBS)
START:
	@echo Compilationing started...
END:
	@echo Compilationing finished...
RUN: $(PROG)
	@echo Runing program....
	./$(PROG)
.PHONY: CLEAN
CLEAN:
	@echo Cleaning....
	rm -vf $(OBJS) $(PROG)
