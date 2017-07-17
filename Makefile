# Makefile for Music Motion
# Last Modified: Edward Ly, 26 September 2016

EXPENDABLES = music-motion sine.o
LIBRARIES = -lm -lGL -lglut -lGLU -lpthread
CPPFLAGS = -Wall -ggdb
CFLAGS = $(CPPFLAGS)

music-motion: music-motion.cpp sine.o
	g++ $(CPPFLAGS) music-motion.cpp sine.o -o music-motion $(LIBRARIES)

sine.o: sine.cpp
	c++ $(CPPFLAGS) -c sine.cpp

clean:
	rm -f $(EXPENDABLES)

all:
	rm -f $(EXPENDABLES)
	@make $(EXPENDABLES)
