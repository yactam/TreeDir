CC=gcc
CFLAGS=-Wall -DNDEBUG $(OPTFLAGS)
DEPS=$(wildcard Headers/*.h)
SRCS=$(wildcard Sources/*.c)
OBJS=$(patsubst Sources/%.c,Sources/%.o,$(SRCS))
EXEC=treedir

all: $(EXEC)

$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJS): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $*.c

clean:
	rm -rf ./Sources/*.o 2> log/makefile.log
	rm $(EXEC) 2> log/makefile.log
	rm -rf log/* 2> log/makefile.log

dev: CFLAGS=-g -Wall $(OPTFLAGS)
dev: all

valgrind:
	valgrind --leak-check=full \
         --log-file=log/valgrind-out.txt \
		 --track-origins=yes \
         ./$(EXEC) $(ARGS) 2> log/program_log.log
run:
	./treedir $(ARGS) 2> log/program_log.log
