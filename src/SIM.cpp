/*
Lisian Shehu
Project1-Cache Simulator
Computer Architecture
November 2019
*/

#include <iostream>
#include <fstream>
#include <math.h>
#include <typeinfo>
#include <stdlib.h>
#include "LRU.h"
#include "FIFO.h"
#include "meta_data.h"
#include "Cache.h"

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
    int totalHits = 0;
    double totalMisses = 0;
    double missRatio = 0.0;
    int writesMem = 0;
    int readsMem = 0;
    bool hit = false;
    int indexHit = 0;
    int maxPosition  = 0;
    bool fullSet = false;
    bool setFull = false;
    string operation;
    string rawByteHexAddress;
   
    // derived values
    int numberOfBlocks = cacheSize/(BLOCK_SIZE_BYTES*associativity);

    //initialize cache arrays
    Cache cache(numberOfBlocks, associativity);
    cache.initialize_cache();
    cache.initialize_metaData();
    cache.initialize_writeMetaData();


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
            for (int i = 0; i < associativity; i++)
            {
                if (cache.cache[setNumber][i] == tagAddress)
                {
                    hit = true;
                    totalHits++;
                    indexHit = i;
                    break;
                }
            }

            if (writePolicy == 0) // write-through
            {
                writesMem++;
            }
            // write-hit in cache
            if (hit)
            {
                if(replacementPolicy == 0)
                    updateMetaData(indexHit, setNumber, numberOfBlocks, associativity, cache.cache, cache.metaData);

                // mark cache block as dirty
                if (writePolicy == 1) // write-back
                {
                    for (int i = 0; i < associativity; i++)
                    {
                        if (cache.cache[setNumber][i] == tagAddress)
                        {
                            cache.writeMetaData[setNumber][i] = 1;
                            break;
                        }
                    }
                }
            }
            else if (!hit) // write-miss in cache
            {
                readsMem++;
                totalMisses++;
                if (!setFull)
                {
                    if (replacementPolicy == 0) // LRU replacement
                        fullSet = insertIntoFreeCacheLRU(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);   

                     if (replacementPolicy == 1) // FIFO replacement
                        fullSet = insertIntoFreeCacheFIFO(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);
                }

                // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
                if (fullSet)
                {
                    int writeBack = 0;

                    if (replacementPolicy == 0) // LRU replacement
                        writeBack = insertIntoFullCacheLRU(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);
                    
                    if (replacementPolicy == 1) // FIFO replacement
                        writeBack = insertIntoFullCacheFIFO(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);
                   
                    if (writePolicy == 1 && writeBack == 1)
                    {
                        writesMem++;
                    }
                }
            }
            // reset variables
            setFull = false; 
            maxPosition = 0;
            hit = false;
        }
        else if (operation == "R") // read operation
        {   
            for (int i = 0; i < associativity; i++)
            {
                if (cache.cache[setNumber][i] == tagAddress)
                {
                    hit = true;
                    totalHits++;
                    indexHit = i;
                    break;
                } 
            }
            // update metadata for a hit
            if (hit)
            {
                if(replacementPolicy == 0)
                    updateMetaData(indexHit, setNumber, numberOfBlocks, associativity, cache.cache, cache.metaData);            
            }
            else if (!hit) // perform cache miss process
            {
                bool fullSet;
                readsMem++;
                totalMisses++;
                if (!setFull)
                {
                    if (replacementPolicy == 0) // LRU replacement
                        fullSet = insertIntoFreeCacheLRU(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);   

                    if (replacementPolicy == 1) // FIFO replacement
                        fullSet = insertIntoFreeCacheFIFO(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);
                }

                // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
                if (fullSet)
                {
                    int writeBack = 0;

                    // LRU replacement
                    if (replacementPolicy == 0) 
                        writeBack = insertIntoFullCacheLRU(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);
                    
                    // FIFO replacement
                    if (replacementPolicy == 1) 
                        writeBack = insertIntoFullCacheFIFO(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache.cache, cache.metaData, cache.writeMetaData);
                    
                    // writes memory if a write back was triggered
                    if (writePolicy == 1 && writeBack == 1) 
                    {
                        writesMem++;
                    }
                }
            }
            // reset variables
            setFull = false;
            maxPosition = 0;
            hit = false;
        }
    }

    tracefile.close();

    missRatio = totalMisses/totalAccesses;
    cout << "\n";
    cout << "Miss ratio: ";
    cout << missRatio << endl;
    cout << "Writes to memory: ";
    cout << writesMem << endl;
    cout << "Reads: ";
    cout << readsMem << endl;

}











    

