CC=gcc
LD=gcc

TARGET=calc

CFLAGS=-g -Wall -o $(TARGET)
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-2.0`
LDFLAGS=

SRCS=calc.c rpn.c

OBJS=$(addsuffix .o, $(SRCS))
MOBJS=$(addprefix obj/, $(OBJS))

$(TARGET): $(OBJS) 
	$(LD) $(LDFLAGS) $(MOBJS) -o $(TARGET) $(GTKFLAGS)

%.c.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o obj/$@ $(GTKFLAGS)

clean:
	rm $(MOBJS) $(TARGET)

