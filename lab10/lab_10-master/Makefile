# Pelda Makefile a vektor feladat megoldasanak forditasara 
# Solaris (ural2) es Linux ala.

# forditando obj. fajlok
objs = vektor_test.o

# headerek, melyektol az egyszeruseg kedveert minden fugg
heads = gen_array_iter3.hpp vektor.hpp mystack.hpp

prog = vektor_test         	    # a program neve

CXX = g++                           # a C++ fordito neve
CXXFLAGS = -pedantic -Wall -std=c++11 # C++ kapcsolok: legyen bobeszedu, 
CXXFLAGS += -g                      # ... es forduljon debug info is

# alapertelmezett cel: tesztprogram linkelese
all: $(prog)

$(prog): $(objs) $(objs2)
	$(CXX) $(objs) $(objs2) -o $@

$(objs): $(heads)

# takaritas igeny szerint
clean:
	rm -f $(objs) $(prog)
