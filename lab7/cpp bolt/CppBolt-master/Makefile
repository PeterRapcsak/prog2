#
#
# Makefile a CppBolt eloallitasahoz
# 	kihazsnalja, hogy gmake
#

PROG = boltMain
SRCS = boltMain.cpp datum.cpp kassza.cpp memtrace.cpp string5.cpp
HDRS = aru.h datum.h gtest_lite.h kassza.h memtrace.h tej.h string5.h kifli.h
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -ggdb -pedantic -Wall -Werror -DMEMTRACE -DKIFLI_IS_VAN

$(PROG): $(OBJS) 
	$(CXX) -o $(PROG) $(OBJS)

.PHONY:
clean:
	rm -f $(OBJS) $(PROG)

# Egyszerusites: Minden .o fugg minden header-tol, es meg a Makefile-tol is 
$(OBJS): $(HDRS) Makefile


