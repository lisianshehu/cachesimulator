CXX = g++
CXXFLAGS = -Wall -g
TARGET = sim

$(TARGET): SIM.o Cache.o FIFO.o LRU.o meta_data.o
	$(CXX) $(CXXFLAGS) -o sim SIM.o Cache.o FIFO.o LRU.o meta_data.o

SIM.o: SIM.cpp Cache.h FIFO.h LRU.h meta_data.h
	$(CXX) $(CXXFLAGS) -c SIM.cpp

Cache.o: Cache.h

FIFO.o: FIFO.h meta_data.h

LRU.o: LRU.h meta_data.h

meta_data.o: meta_data.h

.PHONY: clean run

run : 
	./sim 32768 8 0 1 ../tests/XSBENCH.t 
	./sim 32768 8 0 0 ../tests/XSBENCH.t

clean :
	-rm *.o $(objects)
	-rm $(TARGET)