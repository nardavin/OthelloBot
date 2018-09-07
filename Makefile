CC          = g++
CFLAGS      = -std=c++11 -Wall -pedantic -pg -O3 -I/usr/local/include/Eigen
#CFLAGS = -std=c++11 -Wall -pedantic -O2
OBJDIR      = obj
_OBJS       = player.o board.o boardNode.o linearHeuristic.o
OBJS        = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

PLAYERNAME  = sudormrf

all: $(PLAYERNAME) testgame

$(PLAYERNAME): $(OBJS) $(OBJDIR)/wrapper.o
	$(CC) -o $@ $^

testgame: $(OBJDIR)/testgame.o
	$(CC) -o $@ $^

testminimax: $(OBJS) $(OBJDIR)/testminimax.o
	$(CC) -o $@ -pg $^

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) -x c++ $< -o $@

java:
	make -C java/

cleanjava:
	make -C java/ clean

clean:
	rm -f $(OBJDIR)/*.o $(PLAYERNAME) testgame testminimax gmon.out

.PHONY: java testminimax
