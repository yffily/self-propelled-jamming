
CXX   = g++
FLAGS = -Wall -O4 # -g # -ffast-math # 
LIB   = # -lboost_filesystem -lboost_system #  -lm -lstdc++  # -L/usr/local/lib -L/usr/lib
INC   = # -I/usr/lib # -I/usr/include  -I/usr/local/include
CPPFLAGS = $(INC) $(LIB) $(FLAGS)

HEADERS = $(shell find src -name "*.h")
SRC = $(shell find src -name "*.cpp")
OBJ = $(patsubst %.cpp, %.o, $(SRC))
BIN = execute
TMP = $(shell find . -name "src/*~")
DEP = $(patsubst %.cpp, %.d, $(SRC))

#--------------------------------------------------------
# Rules for building objects and executable

$(BIN) : $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(BIN) $(OBJ)

%.o : %.cpp Makefile
	$(CXX) $(CPPFLAGS) -c $< -o $@

#--------------------------------------------------------
# Automatic dependencies: for each %.cpp, make a %.d that
# lists the dependencies of %.cpp, then paste the contents
# of all %.d files into the makefile.

%.d : %.cpp
	@set -e; rm -f $@; \
	$(CXX) $(CPPFLAGS) -MM -MT $*.o $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

-include $(DEP)

#--------------------------------------------------------

clean :
	rm -f $(OBJ) $(DEP) $(TMP) $(BIN)

cleanobj :
	rm -f $(OBJ)

cleandep :
	rm -f $(DEP)
	
new :
	make clean
	make


