HEADERS=$(wildcard *.h)
SRCS=$(wildcard *.c)
OBJS=$(subst .c,.o,$(SRCS))
OPTS=-Werror -Wall -ftrapv -g

.PHONY: clean

my_book_manager: $(OBJS) $(SRCS) $(HEADERS)
	gcc $(OPTS) $(OBJS) -o $@

%.o: %.c %.h
	gcc -c $(OPTS) $<

clean:
	rm -f $(OBJS) my_book_manager
