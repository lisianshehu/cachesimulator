#include "LRU.h"
#include "meta_data.h"
using namespace std;

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