#
# Makefile pelda a pstring feladat megoldasanak forditasara 
# gnumake valtozat
# 	Linuxokon es ural2-n is elerheto
#
PROG = hetero_test   # a program neve (ezt allitjuk elo)
PROG_O = hetero_test.o # program object fajljai 
PROG_H = hetero_store.hpp bacterium.h memtrace/gtest_lite.h testclass.h # program header fajljai
PROG_L =              # program libjei

MTRACE_O = memtrace/memtrace.o # memtrace object fajl
MTRACE_H = memtrace/memtrace.h # memtrace es memcheck header fajlja

CXX = g++   # a C fordito neve
#CXX = clang++ # clang-ot (llvm) is erdemes kiprobalni
CXXFLAGS = -pedantic -Wall -Werror -std=c++11 # kapcsolok: legyen bobeszedu es pedans
CXXFLAGS += -ggdb -DMEMTRACE # es legyen debug info is
LDFLAGS = -ggdb # debug a linkelesnel 

# osszes object, osszes header osszes lib
OBJS = $(PROG_O) $(MTRACE_O)
HEADS = $(PROG_H) $(MTRACE_H)
LIBS = $(PROG_L) 

# alapertelmezett cel: program
.PHONY: all
all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# feltetelezzuk, hogy az osszes obj fugg az osszes headertol
$(OBJS): $(HEADS)

# takaritas igeny szerint
.PHONY: clean
clean:
	rm -f $(OBJS) $(PROG)

