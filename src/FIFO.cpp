#include "FIFO.h"
using namespace std;

bool insertIntoFreeCacheFIFO(Cache *cache, std::string operation, int set_number, unsigned long long int tagAddress)
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
                cache->cache[set_number][i] = tagAddress;
                int newDataIndex = i;
                cache->metaData[set_number][i] = newDataIndex+1;
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

// inserts into cache using FIFO replacement with a full set
int insertIntoFullCacheFIFO(Cache *cache, std::string operation, int set_number, unsigned long long int tagAddress)
{
    int FIFOIndex = 0;
    int writeBack = 0;
    for (int i = 0; i < cache->associativity; i++)
    {
        if (cache->metaData[set_number][i] == 1)
        {
            FIFOIndex = i;
        }
    }
    cache->cache[set_number][FIFOIndex] = tagAddress;
    for (int i = 0; i < cache->associativity; i++)
    {
        if (cache->metaData[set_number][i] > 1)
        {
            cache->metaData[set_number][i]--;
        }
    }

    // write-back algorithim for writing to memory if there is a dirty bit in place
    if (cache->writePolicy == 1)
    {
        writeBack = writeBackDataCheck(cache, set_number, FIFOIndex, tagAddress);
        if (operation == "R")
        {
            cache->writeMetaData[set_number][FIFOIndex] = 0; // sets dirty bit to 0 for a read-miss when new cache block is allocated
        }
        else if (operation == "W")
        {
            cache->writeMetaData[set_number][FIFOIndex] = 1; // sets dirty bit to 1 for a write-miss when new cache block is allocated
        }
    }
    cache->metaData[set_number][FIFOIndex] = cache->associativity;
    return writeBack;
}
