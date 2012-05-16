
CPPFILES=$(wildcard *.cpp)
HPPFILES=$(wildcard *.hpp)
OFILES_DEBUG=$(patsubst %.cpp,bin/debug/%.o,$(CPPFILES))
OFILES_PROD=$(patsubst %.cpp,bin/prod/%.o,$(CPPFILES))

GCCOPTS=-Wall
DEBUGOPTS=-DDEBUG -g
PRODOPTS=-O2

all: tracktempotest

tracktempotest:	tracktempotest.cpp
	g++ tracktempotest.cpp ../tracktempo.cpp -o tracktempotest.x
