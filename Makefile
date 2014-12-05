CC=g++
CFLAGS= -c -Wall
SOURCES=boggle.cpp
INCLUDE = -I/usr/include
LIBS= -L/usr/lib64 -lncurses -lcdkw -lmenu

OBJECTS=$(SOURCES:.cpp=.o)
MAIN=boggle

all: $(SOURCES) $(MAIN)
		
$(MAIN): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $(MAIN) $(OBJS) $(LIBS)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@
