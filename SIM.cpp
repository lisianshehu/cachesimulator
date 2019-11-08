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
using namespace std;
void updateMetaData(int indexHit, int setNumber, int numberOfBlocks, int associativity, unsigned long long int **cache, int **metaData);
bool insertIntoFreeCacheLRU(string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
int insertIntoFullCacheLRU(string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
int writeBackDataCheck(int writesMem, int setNumber, int numberOfBlocks, int associativity, int LRUIndex, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
bool insertIntoFreeCacheFIFO(string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
int insertIntoFullCacheFIFO(string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);

int main(int argc, char** argv)
{
    // read in arguments 
    int cacheSize = atoi(argv[1]);
    int associativity = atoi(argv[2]);
    int replacementPolicy = atoi(argv[3]);
    int writePolicy = atoi(argv[4]);
    string tracefilepath = argv[5];

    // constants for any cache input arguments 
    int addressSizeBits = 64;
    int wordSizeBytes = 16;
    int blockSizeBytes = 64;
    
    // data variables
    double totalAccesses = 0;
    int totalHits = 0;
    double totalMisses = 0;
    double missRatio = 0.0;
    int writesMem = 0;
    int readsMem = 0;
    bool hit = false;
    int indexHit = 0;
    int maxPosition  = 0;
    bool setFull = false;

    // derived values
    int numberOfBlocks = cacheSize/(blockSizeBytes*associativity);

    //initialize cache arrays
    unsigned long long int **cache;
    cache = new unsigned long long int*[numberOfBlocks];
    for (int i = 0; i < numberOfBlocks; i++)
    {
        cache[i] = new unsigned long long int[associativity];
    }
    for (int i = 0; i < numberOfBlocks; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j] = -1;
        }
    }

    int **metaData;
    metaData = new int*[numberOfBlocks];
    for (int i = 0; i < numberOfBlocks; i++)
    {
        metaData[i] = new int[associativity];
    }
    for (int i = 0; i < numberOfBlocks; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            metaData[i][j] = -1;
        }
    }
    int **writeMetaData;
    writeMetaData = new int*[numberOfBlocks];
     for (int i = 0; i < numberOfBlocks; i++)
    {
        writeMetaData[i] = new int[associativity];
    }
    for (int i = 0; i < numberOfBlocks; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            writeMetaData[i][j] = 0;
        }
    }


    ifstream tracefile;
    tracefile.open(tracefilepath);
    string operation;
    string rawByteHexAddress;

    while (tracefile)
    {
        totalAccesses++;
        getline(tracefile, operation, ' ');
        operation.erase(remove(operation.begin(), operation.end(), '\n'), operation.end());
        tracefile >> rawByteHexAddress;
        unsigned long long int byteAddressDec = stoul(rawByteHexAddress, 0, 16);
        unsigned long long int blockAddress = byteAddressDec/blockSizeBytes;
        int setNumber = blockAddress % numberOfBlocks;
        unsigned long long int tagAddress = (byteAddressDec/blockSizeBytes);

        // write operation
        if (operation == "W")
        {
            for (int i = 0; i < associativity; i++)
            {
                if (cache[setNumber][i] == tagAddress)
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
                    updateMetaData(indexHit, setNumber, numberOfBlocks, associativity, cache, metaData);

                // mark cache block as dirty
                if (writePolicy == 1) // write-back
                {
                    for (int i = 0; i < associativity; i++)
                    {
                        if (cache[setNumber][i] == tagAddress)
                        {
                            writeMetaData[setNumber][i] = 1;
                            break;
                        }
                    }
                }
            }
            else if (!hit) // write-miss in cache
            {
                bool fullSet;
                readsMem++;
                totalMisses++;
                if (!setFull)
                {
                    if (replacementPolicy == 0) // LRU replacement
                        fullSet = insertIntoFreeCacheLRU(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache, metaData, writeMetaData);   

                     if (replacementPolicy == 1) // FIFO replacement
                        fullSet = insertIntoFreeCacheFIFO(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache, metaData, writeMetaData);
                }

                // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
                if (fullSet)
                {
                    int writeBack = 0;

                    if (replacementPolicy == 0) // LRU replacement
                        writeBack = insertIntoFullCacheLRU(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache, metaData, writeMetaData);
                    
                    if (replacementPolicy == 1) // FIFO replacement
                        writeBack = insertIntoFullCacheFIFO(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache, metaData, writeMetaData);
                   
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
                if (cache[setNumber][i] == tagAddress)
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
                    updateMetaData(indexHit, setNumber, numberOfBlocks, associativity, cache, metaData);            
            }
            else if (!hit) // perform cache miss process
            {
                bool fullSet;
                readsMem++;
                totalMisses++;
                if (!setFull)
                {
                    if (replacementPolicy == 0) // LRU replacement
                        fullSet = insertIntoFreeCacheLRU(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache, metaData, writeMetaData);   

                    if (replacementPolicy == 1) // FIFO replacement
                        fullSet = insertIntoFreeCacheFIFO(operation, writePolicy, setFull, setNumber, numberOfBlocks, associativity, tagAddress, cache, metaData, writeMetaData);
                }

                // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
                if (fullSet)
                {
                    int writeBack = 0;

                    // LRU replacement
                    if (replacementPolicy == 0) 
                        writeBack = insertIntoFullCacheLRU(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache, metaData, writeMetaData);
                    
                    // FIFO replacement
                    if (replacementPolicy == 1) 
                        writeBack = insertIntoFullCacheFIFO(operation, writePolicy, writesMem, setNumber, numberOfBlocks, associativity, maxPosition, tagAddress, cache, metaData, writeMetaData);
                    
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

// updates metadata for a read/write hit
void updateMetaData (int indexHit, int setNumber, int numberOfBlocks, int associativity, unsigned long long int **cache, int **metaData)
{
    for (int i = 0; i < associativity; i++)
    {
        if ((metaData[setNumber][i] < metaData[setNumber][indexHit]))
        {
            metaData[setNumber][i]++;
        }
    }
    metaData[setNumber][indexHit] = 0;
}

// inserts when there is free space in cache for LRU replacement 
bool insertIntoFreeCacheLRU (string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    if (cache[setNumber][associativity-1] != -1)
    {
        return true;
    }
    else 
    {
        for (int i = 0; i < associativity; i++)
        {
            if (cache[setNumber][i] == -1)
            {
                cache[setNumber][i] = tagAddress;
                int newDataIndex = i;
                // setFull = false;
                for (int j = 0; j < associativity; j++)
                {
                    if (metaData[setNumber][j] > -1)
                    {
                        metaData[setNumber][j]++;
                    }
                }
                metaData[setNumber][newDataIndex] = 0;
                if (operation == "W" && writePolicy == 1)
                {
                    writeMetaData[setNumber][newDataIndex] = 1;
                } 
                break;
            } 
        }
    }
    return false;
}

// inserts into cache that is full with LRU replacement 
int insertIntoFullCacheLRU (string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    int writeBack = 0;
    int LRUIndex = 0;
    for (int i = 0; i < associativity; i++)
    {
        if (metaData[setNumber][i] > maxPosition)
        {
            maxPosition = metaData[setNumber][i];
            LRUIndex = i;
        }
    }
    cache[setNumber][LRUIndex] = tagAddress;
    for (int i = 0; i < associativity; i++)
    {
        if ((metaData[setNumber][i] < metaData[setNumber][LRUIndex]))
        {
            metaData[setNumber][i]++;
        }
    }

    // write-back algorithim for writing to memory if there is a dirty bit in place
    if (writePolicy == 1)
    {
        writeBack = writeBackDataCheck(writesMem, setNumber, numberOfBlocks, associativity, LRUIndex, tagAddress, cache, metaData, writeMetaData);
        if (operation == "R")
        {
            writeMetaData[setNumber][LRUIndex] = 0; // sets dirty bit to 0 for a read-miss when new cache block is allocated
        }
        else if (operation == "W")
        {
            writeMetaData[setNumber][LRUIndex] = 1; // sets dirty bit to 1 for a write-miss when new cache block is allocated
        }
    }
    metaData[setNumber][LRUIndex] = 0;
    return writeBack;
}

bool insertIntoFreeCacheFIFO(string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    if (cache[setNumber][associativity-1] != -1)
    {
        return true;
    }
    else 
    {
        for (int i = 0; i < associativity; i++)
        {
            if (cache[setNumber][i] == -1)
            {
                cache[setNumber][i] = tagAddress;
                int newDataIndex = i;
                // setFull = false;
                metaData[setNumber][i] = newDataIndex+1;
                if (operation == "W" && writePolicy == 1)
                {
                    writeMetaData[setNumber][newDataIndex] = 1;
                } 
                break;
            } 
        }
    }
    return false;
}

// inserts into cache using FIFO replacement with a full set
int insertIntoFullCacheFIFO(string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    int FIFOIndex = 0;
    int writeBack = 0;
    for (int i = 0; i < associativity; i++)
    {
        if (metaData[setNumber][i] == 1)
        {
            FIFOIndex = i;
        }
    }
    cache[setNumber][FIFOIndex] = tagAddress;
    for (int i = 0; i < associativity; i++)
    {
        if (metaData[setNumber][i] > 1)
        {
            metaData[setNumber][i]--;
        }
    }

    // write-back algorithim for writing to memory if there is a dirty bit in place
    if (writePolicy == 1)
    {
        writeBack = writeBackDataCheck(writesMem, setNumber, numberOfBlocks, associativity, FIFOIndex, tagAddress, cache, metaData, writeMetaData);
        if (operation == "R")
        {
            writeMetaData[setNumber][FIFOIndex] = 0; // sets dirty bit to 0 for a read-miss when new cache block is allocated
        }
        else if (operation == "W")
        {
            writeMetaData[setNumber][FIFOIndex] = 1; // sets dirty bit to 1 for a write-miss when new cache block is allocated
        }
    }
    metaData[setNumber][FIFOIndex] = associativity;
    return writeBack;
}

// checks for dirty bit to write back
int writeBackDataCheck (int writesMem, int setNumber, int numberOfBlocks, int associativity, int replcIndex, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    if (writeMetaData[setNumber][replcIndex] == 1)
    {
        return 1;
    }

    return 0;
}