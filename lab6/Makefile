all: compile

compile:
	g++ -o main -fopenmp main.cpp

run: main.o
	./main

clean:
	rm -rf *.o