CC=gcc
CFLAGS=-c -Wall
SOURCES=main.c algo.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=result

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(OBJECTS)