# Pelda Makefile a 3. labor megoldasanak forditasara es tesztelesere
# Solaris (ural2) es Linux ala.

OBJECTS = komplex_teszt.o komplex.o # forditando obj. fajlok
HEADERS = gtest_lite.h elkeszult.h komplex.h
#
CXX = g++                       # a C++ fordito neve
#CXX = clang++                  # clang-ot (llvm) is erdemes kiprobalni
CXXFLAGS = -pedantic -Wall      # C++ kapcsolok: legyen bobeszedu, 
CXXFLAGS += -g 			# debug info 

TARGET	= komplex_teszt

# alapertelmezett cel: tesztek futtatasa
.PHONY:
do_test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(OBJECTS): $(HEADERS)

$(TARGET): $(OBJECTS) $(HEADERS)	# nem feltétlen figg mindenki minden headertol
	$(CXX) $(OBJECTS) -o $@

# takaritas igeny szerint
.PHONY:
clean:
	rm -f *.o $(TARGET) *~ *.cbp *.vcxproj*

# tar igeny szerint
.PHONY:
tar:
	tar -czvf komplex.tgz *.h *.cpp 

