CXX = g++
CXXFLAGS = -Wall -g
TARGET = sim

$(TARGET): SIM.o Cache.o FIFO.o LRU.o meta_data.o read_operations.o write_operations.o
	$(CXX) $(CXXFLAGS) -o sim SIM.o Cache.o FIFO.o LRU.o meta_data.o read_operations.o write_operations.o

SIM.o: SIM.cpp Cache.h FIFO.h LRU.h meta_data.h read_operations.h write_operations.h
	$(CXX) $(CXXFLAGS) -c SIM.cpp

Cache.o: Cache.h read_operations.h write_operations.h

FIFO.o: FIFO.h meta_data.h

LRU.o: LRU.h meta_data.h

meta_data.o: meta_data.h

read_operations.o: Cache.h LRU.h FIFO.h meta_data.h

write_operations.o: Cache.h LRU.h FIFO.h meta_data.h

.PHONY: clean run

clean :
	-rm *.o $(objects)
	-rm $(TARGET)