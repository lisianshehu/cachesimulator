#include "Cache.h"

void Cache::initialize_cache()
{
    cache = new unsigned long long int *[num_blocks];
    for (int i = 0; i < num_blocks; i++)
    {
        cache[i] = new unsigned long long int[associativity];
    }
    for (int i = 0; i < num_blocks; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j] = -1;
        }
    }
}

void Cache::initialize_metaData()
{
    metaData = new int *[num_blocks];
    for (int i = 0; i < num_blocks; i++)
    {
        metaData[i] = new int[associativity];
    }
    for (int i = 0; i < num_blocks; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            metaData[i][j] = -1;
        }
    }
}

void Cache::initialize_writeMetaData()
{
    writeMetaData = new int*[num_blocks];
     for (int i = 0; i < num_blocks; i++)
    {
        writeMetaData[i] = new int[associativity];
    }
    for (int i = 0; i < num_blocks; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            writeMetaData[i][j] = 0;
        }
    }
}

// checks for dirty bit to write back
int Cache::write_back_data_check(Cache *cache, int set_number, int replcIndex, unsigned long long int tag_address)
{
    if (cache->writeMetaData[set_number][replcIndex] == 1)
    {
        return 1;
    }

    return 0;
}