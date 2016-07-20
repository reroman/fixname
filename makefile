CXX := g++
CXXFLAGS := -Wall -O2 -std=c++11
SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:.cpp=.o)

fixname: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	$(RM) $(OBJS)
