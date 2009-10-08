# Set machine-specific stuff

CC=g++ -g -Wall
CFLAGS=-I.

#####################################################################

TARGETS = myprog
SRC = EventStream.cpp EventStream.hpp RandomGeneration.cpp RandomGeneration.hpp test.cpp
OBJS = $(SRC:.cpp=.o)

# Default target
all: $(TARGETS)

# Compilation rules
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Clean
clean:
	rm -f core *.o ${TARGETS} *~
