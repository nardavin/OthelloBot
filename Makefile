CC          = g++
CFLAGS      = -std=c++11 -Wall -pedantic -pg -O3
#CFLAGS = -std=c++11 -Wall -pedantic -O2
OBJS        = player.o board.o boardNode.o heuristics.o
PLAYERNAME  = sudormrf

all: $(PLAYERNAME) testgame

$(PLAYERNAME): $(OBJS) wrapper.o
	$(CC) -o $@ $^

testgame: testgame.o
	$(CC) -o $@ $^

testminimax: $(OBJS) testminimax.o
	$(CC) -o $@ -pg $^

%.o: %.cpp
	$(CC) -c $(CFLAGS) -x c++ $< -o $@

java:
	make -C java/

cleanjava:
	make -C java/ clean

clean:
	rm -f *.o $(PLAYERNAME) testgame testminimax gmon.out

.PHONY: java testminimax
