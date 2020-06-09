#include "meta_data.h"
using namespace std;

void updateMetaData (Cache *cache, int set_number)
{
    for (int i = 0; i < cache->associativity; i++)
    {
        if ((cache->metaData[set_number][i] < cache->metaData[set_number][cache->cache_data.indexHit]))
        {
            cache->metaData[set_number][i]++;
        }
    }
    cache->metaData[set_number][cache->cache_data.indexHit] = 0;
}

// checks for dirty bit to write back
int writeBackDataCheck (Cache *cache, int set_number, int replcIndex, unsigned long long int tag_address)
{
    if (cache->writeMetaData[set_number][replcIndex] == 1)
    {
        return 1;
    }

    return 0;
}