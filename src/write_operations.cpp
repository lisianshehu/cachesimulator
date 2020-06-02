#include "write_operations.h"

void perform_write(Cache *cache, string operation, int set_number, unsigned long long int tag_address)
{
    for(int i = 0; i < cache->associativity; i++)
    {
        if (cache->cache[set_number][i] == tag_address)
        {
            cache->cache_data.hit = true;
            cache->cache_data.totalHits++;
            cache->cache_data.indexHit = i;
            break;
        }

            if (cache->writePolicy == 0) // write-through
            {
                cache->cache_data.writesMem++;
            }
            
            // write-hit in cache
            if (cache->cache_data.hit)
            {
                if(cache->replacementPolicy == 0)
                    updateMetaData(cache, set_number);

                // mark cache block as dirty
                if (cache->writePolicy == 1) // write-back
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
            else if (!cache->cache_data.hit) // write-miss in cache
            {   
                bool setFull = false;
                bool fullSet = false;
                cache->cache_data.readsMem++;
                cache->cache_data.totalMisses++;

                if (!setFull)
                {
                    if (cache->replacementPolicy == 0) // LRU replacement
                        fullSet = insertIntoFreeCacheLRU(cache, operation, set_number, tag_address);   

                    if (cache->replacementPolicy == 1) // FIFO replacement
                        fullSet = insertIntoFreeCacheFIFO(cache, operation, set_number, tag_address);

                    setFull = false;
                }

                // checks for empty sets to place data and sets metadata to MRU data and decide which element to evict
                if (fullSet)
                {
                    int writeBack = 0;

                    if (cache->replacementPolicy == 0) // LRU replacement
                        writeBack = insertIntoFullCacheLRU(cache, operation, set_number, tag_address);
                    
                    if (cache->replacementPolicy == 1) // FIFO replacement
                        writeBack = insertIntoFullCacheFIFO(cache, operation, set_number, tag_address);
                   
                    if (cache->writePolicy == 1 && writeBack == 1)
                    {
                        cache->cache_data.writesMem++;
                    }

                    fullSet = false;
                }
            }
    }

    
}