CC=  g++
CFLAGS=   -c -w -std=c++11
SOURCES=main.cpp Server.cpp Board.cpp
INCLUDE = -I/usr/include
LIBS= -L/usr/lib64 -lncursesw -lcdk -lmenu -lboost_locale -lzmq

OBJECTS=$(SOURCES:.cpp=.o)
MAIN=boggle

all: $(SOURCES) $(MAIN)
		
$(MAIN): $(OBJECTS) 
		$(CC) -g -O0 $(OBJECTS) -o $(MAIN) $(OBJS) $(LIBS)

.cpp.o:
		$(CC) -g -O0 $(CFLAGS) $< -o $@
