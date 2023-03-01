# list of all headers
HEADERS := $(wildcard *.hpp)
# list of all the source files
SOURCES := $(wildcard *.cpp)
# list of all object files by sostitution of all the *.cpp with *.o
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
# list of all the executable files
EXECUTABLES := create_network.x simulation.x prova.x
# list of executable names
EXECUTABLENAMES := $(patsubst %.x,%,$(EXECUTABLES))

# compiler directives
WARNING := -Wall -Wpedantic -Wextra -Wno-sign-compare
CXX := h5c++
CXXFLAGS := -O3 -std=c++17


.PHONY: all clean $(EXECUTABLENAMES)


all: $(EXECUTABLES)

$(EXECUTABLENAMES): % : %.x

%.x: %.o
	$(CXX) $^ -o $@

%.o: %.cpp Makefile $(HEADERS)
	$(CXX) $(WARNING) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLES)