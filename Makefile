CFLAGS= -Wpedantic -std=c++11 -Wall -Wextra

all:
	g++ ${CFLAGS} *.cpp -o arsha.x

clean:
	rm -rf *.out *.o *.x
