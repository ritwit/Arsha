CFLAGS= -Wpedantic -std=c++11 -Wall -Wextra  -g 
all:
	g++ ${CFLAGS} *.cpp -o arsha.x

clean:
	rm -rf *.out *.o *.x
