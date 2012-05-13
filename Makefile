CC=gcc
LD=gcc

TARGET=calc
TEST_TARGET=calc_test

CFLAGS=-g -Wall -o $(TARGET)
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-3.0`
LDFLAGS=
TESTFLAGS=-lcunit

SRCS=calc.c callbacks.c calc_context.c calclist.c rpn.c
TEST_SRCS=rpn.c calc_test.c

OBJS=$(addsuffix .o, $(SRCS))
TEST_OBJS=$(addsuffix .o, $(TEST_SRCS))
MOBJS=$(addprefix obj/, $(OBJS))
MTEST_OBJS=$(addprefix obj/, $(TEST_OBJS))

$(TARGET): $(OBJS) 
	$(LD) $(LDFLAGS) $(MOBJS) -o $(TARGET) $(GTKFLAGS)

%.c.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o obj/$@ $(GTKFLAGS)

%.c.o: test/%.c
	$(CC) -c $(CFLAGS) $< -o obj/$@ $(TESTFLAGS)

clean:
	rm obj/* $(TARGET) $(TEST_TARGET)

test: $(TEST_OBJS)
	$(CC) $(MTEST_OBJS) -o $(TEST_TARGET) -lcunit
	./$(TEST_TARGET)
