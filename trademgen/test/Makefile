# Set machine-specific stuff

CC=g++ -g -Wall
CFLAGS=-I.

#####################################################################

TARGETS = myprog
SRC = EventStream.cpp RandomGeneration.cpp CategoricalAttribute.cpp test.cpp
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
