CXX=clang++
OPT=-O3
STD=-std=c++20
DEPFLAGS=-MMD -MP
CXXFLAGS=-Wall -Wextra -Iinclude $(OPT)


CPPFILES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(CPPFILES))
DEPFILES=$(patsubst src/%.cpp,obj/%.d,$(CPPFILES))

BINARY=bin/tetris
BINFLAGS=-lGL -lGLU -lglut

all: $(BINARY)
debug: CXXFLAGS+= -pg
debug: $(BINARY)


$(BINARY): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(BINFLAGS) -o $(BINARY) $(OBJECTS)

obj/%.o:src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c -o $@ $<

obj/main.o:src/main.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS)

-include $(DEPFILES)