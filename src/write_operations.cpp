#include "write_operations.h"

void write_operation_control(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
{
    for(int i = 0; i < cache->associativity; i++)
    {
        if (cache->cache[set_number][i] == tag_address)
        {
            cache->cache_data.hit = true;
            cache->cache_data.indexHit = i;
            break;
        }
    }

    if (cache->write_policy == 0) // write-through
    {
        cache->cache_data.writesMem++;
    }

    // write-hit in cache
    if (cache->cache_data.hit)
    {
        write_hit(cache, set_number, tag_address);
    }
    else if (!cache->cache_data.hit) // write-miss in cache
    {   
        write_miss(cache, operation, set_number, tag_address);
    }
}

void write_hit(Cache *cache, int set_number, unsigned long long int tag_address)
{
    cache->update_meta_data(cache, set_number);

    // mark cache block as dirty
    if (cache->write_policy == 1) // write-back
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->cache[set_number][i] == tag_address)
            {
                cache->writeMetaData[set_number][i] = 1;
                break;
            }
        }
    }
}

void write_miss(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address)
{
    bool setFull = false;
    bool fullSet = false;
    cache->cache_data.readsMem++;
    cache->cache_data.totalMisses++;

    if (!setFull)
    {
        fullSet = cache->insert_into_free_cache(cache, operation, set_number, tag_address);

        setFull = false;
    }

    // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
    if (fullSet)
    {
        int writeBack = 0;

        writeBack = cache->insert_into_full_cache(cache, operation, set_number, tag_address);
        
        if (cache->write_policy == 1 && writeBack == 1)
        {
            cache->cache_data.writesMem++;
        }

        fullSet = false;
    }
}