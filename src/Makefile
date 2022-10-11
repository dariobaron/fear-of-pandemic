# list of all headers
HEADERS := $(wildcard *.hpp)
# list of all the source files
SOURCES := $(wildcard *.cpp)
# list of all object files by sostitution of all the *.cpp with *.o
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
# list of all the executable files
EXECUTABLES := create_network.x simulation.x

# compiler directives
WARNING := -Wall -Wpedantic -Wextra -Wno-sign-compare
CXX := g++
CXXFLAGS := -O3


.PHONY: all clean


all: $(EXECUTABLES)

%.x: %.o
	$(CXX) $^ -o $@

%.o: %.cpp Makefile $(HEADERS)
	$(CXX) $(WARNING) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLES)