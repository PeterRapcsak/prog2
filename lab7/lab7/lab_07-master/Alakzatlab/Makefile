#
# Makefile az alakzatlab (7. labor) megoldasanak forditasara
# gnumake valtozat
# 	Linuxokon es ural2-n is elerheto
#
# A program neve (ezt allitjuk elo)
PROG = alakzat_main

# Rajzatablas valtozathoz torolje a kommentet a kovetkezo sorbol
PROG = alakzat2_main
PROG_O = $(PROG).o alakzat.o kor.o szakasz.o szin.o pont.o haromszog.o # program object fajljai

# Rajzatablas valtozathoz torolje a kommentet a kovetkezo sorbol
PROG_O += rajztabla.o
PROG_H = alakzat.h kor.h szakasz.h ellipszis.h szin.h pont.h haromszog.h # program header fajljai

# Rajzatablas valtozathoz torolje a kommentet a kovetkezo sorbol
PROG_H += rajztabla.h

PROG_L =              # program libjei
MTRACE_O = memtrace.o # memtrace object fajl
MTRACE_H = memtrace.h # memtrace header fajlja

CXX = g++ # a C fordito neve

#fordítasi opciok
CXXFLAGS = -pedantic -Wall -DMEMTRACE
CXXFLAGS += -g # es legyeb debug info is
LDFLAGS = -g # debug a linkelesnel is kell

# osszes object, osszes header osszes lib egyutt
OBJS = $(PROG_O) $(MTRACE_O)
HEADS = $(PROG_H) $(MTRACE_H)
LIBS = $(PROG_L) $(MEMCHK_L)

# Alapertelmezett cel 
.PHONY: all
all: $(PROG)

# Eloallitjuk az objektekbol a PROG-ot
$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# Feltetelezzuk, hogy az osszes obj fugg az osszes headertol, ami nem feltetlenul igaz
$(OBJS): $(HEADS)

# Futtatas
.PHONY: run
run: $(PROG)
	./$(PROG)

# takaritas igeny szerint
.PHONY: clean
clean:
	rm -f $(OBJS) $(PROG)

