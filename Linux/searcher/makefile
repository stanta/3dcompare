CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES= search.cpp stlcompare.cpp stlfile.cpp stlsphere.cpp vector.cpp mgcgiin.cpp mgcgiout.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=search

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
