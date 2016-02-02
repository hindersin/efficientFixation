# C++ Compiler
CCXX=g++

# Compilling common flags
LANG=en_US.ISO-8859-1
CFLAGS=-c -Wall -std=c++0x -Wno-unused $(IDIR)

# Binary Dir
BDIR=

# Source Path
SDIR=./source/

# Default build is release
BUILD_TYPE=release

ifeq ($(BUILD_TYPE),debug)
CFLAGS+=-ggdb -DDEBUG -O0
	BDIR=./debug/
endif

ifeq ($(BUILD_TYPE),release)
	CFLAGS+=-DNDEBUG -O3 -funroll-loops
	 BDIR=./release/
endif
    
# Include Paths
#IDIR=-I./$(SDIR)
IDIR=-I /usr/local/igraph
IDIR+=-I /opt/local/include

# Library Paths
LDIR=-L /usr/local/lib 
LDIR+=-L/opt/local/lib

# Libraries
LIBS=-ligraph
#LIBS+= -lgsl -lgslcblas -lm -lstdc++
#LIBS+=-lboost_date_time-mt

LDFLAGS=$(LDIR) $(LIBS)

# Sources
SOURCES=$(SDIR)solveMatrix.cpp
SOURCES+=$(SDIR)transitionMatrixshort.cpp
SOURCES+=$(SDIR)conditionalMatrix.cpp
SOURCES+=$(SDIR)main.cpp

# Binary
BINARY=$(BDIR)fixating.out

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(BINARY)
	
$(BINARY): $(OBJECTS) 
	$(info --- LINKING ---)
	$(CCXX) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(info --- COMPILLING FILE: $@)
	$(CCXX) $(CFLAGS) $< -o $@
	
.PHONY: copy

copy:
	$(info --- MOVING OBJECTS ---)
	cp $(OBJECTS)  $(BDIR)
	rm -f $(OBJECTS) 

clean: 
	$(info --- CLEANING ---)
	rm -f $(BDIR)/*.o
	rm -f $(OBJECTS) $(BINARY)
	

