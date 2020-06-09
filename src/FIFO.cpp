#include "FIFO.h"
using namespace std;


bool FIFOCache::insert_into_free_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
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
                cache->metaData[set_number][i] = newDataIndex+1;
                if (operation == "W" && cache->write_policy == 1)
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
int FIFOCache::insert_into_full_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
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
    cache->cache[set_number][FIFOIndex] = tag_address;
    for (int i = 0; i < cache->associativity; i++)
    {
        if (cache->metaData[set_number][i] > 1)
        {
            cache->metaData[set_number][i]--;
        }
    }

    // write-back algorithim for writing to memory if there is a dirty bit in place
    if (cache->write_policy == 1)
    {
        writeBack = writeBackDataCheck(cache, set_number, FIFOIndex, tag_address);
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

void FIFOCache::updateMetaData(Cache *cache, int set_number)
{
    // no need to update meta data with FIFO replacement policy
    return;
}