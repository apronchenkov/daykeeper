.PHONY: all

CXX = g++
RM = rm -f

CPPFLAGS = 
CXXFLAGS =


all: main.exe

main.exe: main.cc state.h states.h

states.h: genStates.exe
	genStates.exe

genStates.exe: genStates.cc state.h exceptions.h permutation.h 

%.exe : %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

clean : 
	$(RM) main.exe genStates.exe states.h *.o *.obj