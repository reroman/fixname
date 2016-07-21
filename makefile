CXX := g++
PROJECT_NAME := fixname
INSTALL_PATH := /usr/bin/
CXXFLAGS := -Wall -O2 -std=c++11
SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:.cpp=.o)

$(PROJECT_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

install:
	cp -f $(PROJECT_NAME) $(INSTALL_PATH)
uninstall:
	$(RM) $(INSTALL_PATH)$(PROJECT_NAME)

.PHONY: clean

clean:
	$(RM) $(OBJS)
