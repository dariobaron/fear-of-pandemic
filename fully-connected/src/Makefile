# list of all the objects files
OBJECTS = $(wildcard *.o)
# list of all the executable files
EXECUTABLES = fullyconnected.x simulation.x

# compiler directives
WARNING = -Wall -Wpedantic -Wextra -Wno-sign-compare
CXX = g++
CPPFLAGS = -O3 $(WARNING)


all: $(EXECUTABLES)

fullyconnected.x: fullyconnected.o
	$(CXX) $^ -o $@

simulation.x: simulation.o
	$(CXX) $^ -o $@

%.o: %.c
	$(CXX) -c $(CPPFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLES)