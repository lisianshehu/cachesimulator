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
    int cache_size = atoi(argv[1]);
    int associativity = atoi(argv[2]);
    int replacement_policy = atoi(argv[3]);
    int write_policy = atoi(argv[4]);
    string tracefilepath = argv[5];

    double totalAccesses = 0;
    double missRatio = 0.0;
    string operation;
    string rawByteHexAddress;
   
   Cache *cache;
    // derived values
    int numberOfBlocks = cache_size/(BLOCK_SIZE_BYTES*associativity);

    if (replacement_policy == 0)
    {
        cache = new LRUCache(numberOfBlocks, associativity, write_policy);
    }
    else if (replacement_policy == 1)
    {
        cache = new FIFOCache(numberOfBlocks, associativity, write_policy);
    }
    else
    {
        throw "Unrecognized replacement policy";
    }

    //initialize cache arrays
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
        unsigned long long int tag_address = (byteAddressDec/BLOCK_SIZE_BYTES);

        // write operation
        if (operation == "W")
        {
            write_operation_control(cache, operation, setNumber, tag_address);

            // reset variables
            cache->cache_data.hit = false;
        }
        else if (operation == "R") // read operation
        {   
            read_operation_control(cache, operation, setNumber, tag_address);
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

    delete cache;

}











    

