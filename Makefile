CC: gcc
CFLAGS: -Wextra -Wall -g
LDFLAGS = -lpthread

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o);
TARGET: main

$(TARGET):$(OBJECTS)
	$(CC) $(OBJECTS) -o $(CFLAGS)

make:
	cc $(TARGET) -o $(SOURCES) $(CFLAGS) -lpthread

clean:
	rm -f $(TARGET)
