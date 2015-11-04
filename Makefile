OBJS=book.o

all: $(OBJS)

%.o: %.c
	gcc -g  -c -Wall -Werror -Wshadow -ftrapv $<

clean:
	rm -f $(OBJS)
