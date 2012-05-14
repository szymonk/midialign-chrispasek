
CPPFILES=$(wildcard *.cpp)
HPPFILES=$(wildcard *.hpp)
OFILES=$(patsubst %.cpp,%.o,$(CPPFILES))

GCCOPTS=-Wall -O2

all: midialign

midialign: $(OFILES)
	g++ $(GCCOPTS) $^ -o $@

%.o: %.c $(HPPFILES) Makefile
	g++ $(GCCOPTS) $< -o $@
