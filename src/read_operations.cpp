#include "read_operations.h"

void perform_read(Cache *cache, string operation, int set_number, unsigned long long int tag_address)
{
    bool fullSet = false;
    bool setFull = false;

    for (int i = 0; i < cache->associativity; i++)
    {
        if (cache->cache[set_number][i] == tag_address)
        {
            cache->cache_data.hit = true;
            cache->cache_data.totalHits++;
            cache->cache_data.indexHit = i;
            break;
        } 
    }

    // update metadata for a hit
    if (cache->cache_data.hit)
    {
        if(cache->replacementPolicy == 0)
            updateMetaData(cache, set_number);            
    }
    else if (!cache->cache_data.hit) // perform cache miss process
    {
        cache->cache_data.readsMem++;
        cache->cache_data.totalMisses++;
        if (!setFull)
        {
            if (cache->replacementPolicy == 0) // LRU replacement
                fullSet = insertIntoFreeCacheLRU(cache, operation, set_number, tag_address);   

            if (cache->replacementPolicy == 1) // FIFO replacement
                fullSet = insertIntoFreeCacheFIFO(cache, operation, set_number, tag_address);
        }

        // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
        if (fullSet)
        {
            int writeBack = 0;

            // LRU replacement
            if (cache->replacementPolicy == 0) 
                writeBack = insertIntoFullCacheLRU(cache, operation, set_number, tag_address);
            
            // FIFO replacement
            if (cache->replacementPolicy == 1) 
                writeBack = insertIntoFullCacheFIFO(cache, operation, set_number, tag_address);
            
            // writes memory if a write back was triggered
            if (cache->writePolicy == 1 && writeBack == 1) 
            {
                cache->cache_data.writesMem++;
            }
        }
    }
        // reset variables
        cache->cache_data.hit = false;
    }
}
