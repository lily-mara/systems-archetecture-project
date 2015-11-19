SRCS=io.c book.c
OBJS=$(subst .c,.o,$(SRCS))
TEST_SRCS=$(wildcard tests/*.c)
TEST_OBJS=$(subst .c,.o,$(TEST_SRCS))
MAIN=my_book_manager
OPTS=-Werror -Wall -ftrapv -g
LIBS=-L /usr/local/lib/ -lcmocka

.PHONY: clean run_tests

$(MAIN): $(MAIN).o $(OBJS) $(SRCS)
	gcc $(MAIN).o $(OPTS) $(OBJS) -o $@ $(LIBS)

test: $(OBJS) $(SRCS) $(TEST_SRCS) $(TEST_OBJS)
	gcc $(OPTS) $(OBJS) $(TEST_OBJS) -o $@ $(LIBS)

run_tests: test
	./test
	rm -f *.test.dat

%.o: %.c %.h
	gcc -c $(OPTS) $< $(LIBS)

%.o: %.c
	gcc -c $(OPTS) $< $(LIBS) -o $@

clean:
	rm -f $(OBJS) my_book_manager test $(TEST_OBJS)
