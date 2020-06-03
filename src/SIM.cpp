#include <iostream>
#include <fstream>
#include <math.h>
#include <typeinfo>
#include <stdlib.h>
#include "LRU.h"
#include "FIFO.h"
#include "meta_data.h"
#include "Cache.h"
#include "read_operations.h"
#include "write_operations.h"

using namespace std;

#define ADDRESS_SIZE_BITS 64
#define WORD_SIZE_BYTES 16
#define BLOCK_SIZE_BYTES 64

int main(int argc, char** argv)
{   
    int cacheSize = atoi(argv[1]);
    int associativity = atoi(argv[2]);
    int replacementPolicy = atoi(argv[3]);
    int writePolicy = atoi(argv[4]);
    string tracefilepath = argv[5];

    double totalAccesses = 0;
    double missRatio = 0.0;
    string operation;
    string rawByteHexAddress;
   
    // derived values
    int numberOfBlocks = cacheSize/(BLOCK_SIZE_BYTES*associativity);

    //initialize cache arrays
    Cache *cache = new Cache(numberOfBlocks, associativity, replacementPolicy, writePolicy);
    cache->initialize_cache();
    cache->initialize_metaData();
    cache->initialize_writeMetaData();

    ifstream tracefile;
    tracefile.open(tracefilepath);

    while (tracefile)
    {
        totalAccesses++;
        getline(tracefile, operation, ' ');
        operation.erase(remove(operation.begin(), operation.end(), '\n'), operation.end());
        tracefile >> rawByteHexAddress;
        unsigned long long int byteAddressDec = stoul(rawByteHexAddress, 0, 16);
        unsigned long long int blockAddress = byteAddressDec/BLOCK_SIZE_BYTES;
        int setNumber = blockAddress % numberOfBlocks;
        unsigned long long int tagAddress = (byteAddressDec/BLOCK_SIZE_BYTES);

        // write operation
        if (operation == "W")
        {
            write_operation_control(cache, operation, setNumber, tagAddress);

            // reset variables
            cache->cache_data.hit = false;
        }
        else if (operation == "R") // read operation
        {   
            read_operation_control(cache, operation, setNumber, tagAddress);
        }
    }

    tracefile.close();

    missRatio = cache->cache_data.totalMisses/totalAccesses;
    cout << "\n";
    cout << "Miss ratio: ";
    cout << missRatio << endl;
    cout << "Writes to memory: ";
    cout << cache->cache_data.writesMem << endl;
    cout << "Reads: ";
    cout << cache->cache_data.readsMem << endl;

}











    

