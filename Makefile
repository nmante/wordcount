EXE=wordcount
CFLAGS=-g -std=c++11

all: $(EXE)

$(EXE): main.cpp wordcount.o 
	g++ $(CFLAGS) wordcount.o -o $(EXE) main.cpp

wordcount.o: wordcount.hpp wordcount.cpp
	g++ $(CFLAGS) -c wordcount.cpp

clean:
	rm -rf $(EXE).dSYM $(EXE) *.o
