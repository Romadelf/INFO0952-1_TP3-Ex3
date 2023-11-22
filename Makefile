SRCS = main.c Expression.c Dict.c
OBJS = $(SRCS:%.c=%.o)
CC = gcc
PROG = main


CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99
LDFLAGS = -lm

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.PHONY: clean run

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG)

main.o: main.c dict.h Expression.h
Expression.o: Expression.c Dict.h



