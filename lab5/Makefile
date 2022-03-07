CC=gcc
LIBS=-lpthread
CFLAGS=-c -Wall -I include
INCLUDE_DIR=include
SOURCE_DIR=src
BUILD_DIR=build
DIST_DIR=dist

EXECUTABLE_NAME=lab5

EXECUTABLE=$(DIST_DIR)/$(EXECUTABLE_NAME)

.PHONY : all
all: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD_DIR)/main.o $(BUILD_DIR)/daemon.o $(BUILD_DIR)/logger.o
	$(CC) $^ $(LIBS) -o $@

$(BUILD_DIR)/daemon.o: $(SOURCE_DIR)/daemon.c $(INCLUDE_DIR)/logger.h
	$(CC) $(CFLAGS) $(SOURCE_DIR)/daemon.c -o $@

$(BUILD_DIR)/logger.o: $(SOURCE_DIR)/logger.c
	$(CC) $(CFLAGS) $(SOURCE_DIR)/logger.c -o $@


$(BUILD_DIR)/main.o: $(INCLUDE_DIR)/daemon.h $(SOURCE_DIR)/main.c
	$(CC) $(CFLAGS) $(SOURCE_DIR)/main.c -o $@

.PHONY : clean
clean:
	rm -R $(BUILD_DIR)/* $(DIST_DIR)/*

.PHONY : run
run: $(EXECUTABLE)
	$(EXECUTABLE)