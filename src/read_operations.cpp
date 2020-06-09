#include "read_operations.h"

void read_operation_control(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
{
    for (int i = 0; i < cache->associativity; i++)
    {
        if (cache->cache[set_number][i] == tag_address)
        {
            cache->cache_data.hit = true;
            cache->cache_data.indexHit = i;
            break;
        } 
    }

    // update metadata for a hit
    if (cache->cache_data.hit)
    {
        // meta data is updated only for LRU cache
        cache->updateMetaData(cache, set_number);            
    }
    else if (!cache->cache_data.hit) // perform cache miss process
    {
        read_miss(cache, operation, set_number, tag_address);
    }
    
    // reset variables
    cache->cache_data.hit = false;
}

void read_miss(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
{
    bool fullSet = false;
    bool setFull = false;
    cache->cache_data.readsMem++;
    cache->cache_data.totalMisses++;
    if (!setFull)
    {
        fullSet = cache->insert_into_free_cache(cache, operation, set_number, tag_address);
    }

    // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
    if (fullSet)
    {
        int writeBack = 0;

        writeBack = cache->insert_into_full_cache(cache, operation, set_number, tag_address);
                
        // writes memory if a write back was triggered
        if (cache->write_policy == 1 && writeBack == 1) 
        {
            cache->cache_data.writesMem++;
        }
    }
}
