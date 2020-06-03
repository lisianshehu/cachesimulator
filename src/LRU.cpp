#include "LRU.h"
using namespace std;

// inserts when there is free space in cache for LRU replacement 
bool insertIntoFreeCacheLRU (Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
{
    if (cache->cache[set_number][cache->associativity-1] != -1)
    {
        return true;
    }
    else 
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->cache[set_number][i] == -1)
            {
                cache->cache[set_number][i] = tag_address;
                int newDataIndex = i;
                // setFull = false;
                for (int j = 0; j < cache->associativity; j++)
                {
                    if (cache->metaData[set_number][j] > -1)
                    {
                        cache->metaData[set_number][j]++;
                    }
                }
                cache->metaData[set_number][newDataIndex] = 0;
                if (operation == "W" && cache->writePolicy == 1)
                {
                    cache->writeMetaData[set_number][newDataIndex] = 1;
                } 
                break;
            } 
        }
    }
    return false;
}

// inserts into cache that is full with LRU replacement 
int insertIntoFullCacheLRU (Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
{
    int writeBack = 0;
    int maxPosition = 0;
    int LRUIndex = 0;
    for (int i = 0; i < cache->associativity; i++)
    {
        if (cache->metaData[set_number][i] > maxPosition)
        {
            maxPosition = cache->metaData[set_number][i];
            LRUIndex = i;
        }
    }
    cache->cache[set_number][LRUIndex] = tag_address;
    for (int i = 0; i < cache->associativity; i++)
    {
        if ((cache->metaData[set_number][i] < cache->metaData[set_number][LRUIndex]))
        {
            cache->metaData[set_number][i]++;
        }
    }

    // write-back algorithim for writing to memory if there is a dirty bit in place
    if (cache->writePolicy == 1)
    {
        writeBack = writeBackDataCheck(cache, set_number, LRUIndex, tag_address);
        if (operation == "R")
        {
            cache->writeMetaData[set_number][LRUIndex] = 0; // sets dirty bit to 0 for a read-miss when new cache block is allocated
        }
        else if (operation == "W")
        {
            cache->writeMetaData[set_number][LRUIndex] = 1; // sets dirty bit to 1 for a write-miss when new cache block is allocated
        }
    }
    cache->metaData[set_number][LRUIndex] = 0;
    return writeBack;
}