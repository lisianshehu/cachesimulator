#ifndef FIFO_H
#define FIFO_H
#include <string>
#include "Cache.h"

class FIFOCache : public Cache
{
    public:
        bool insert_into_free_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
        int insert_into_full_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
        void update_meta_data(Cache *cache, int set_number);

    FIFOCache(int num_blocks, int associativity, int write_policy) : Cache(num_blocks, associativity, write_policy){};
    ~FIFOCache()
    {
        for (int i = 0; i < num_blocks; i++)
        {
            delete [] cache[i];
            delete [] metaData[i];
            delete [] writeMetaData[i];
        }

        delete [] cache;
        delete [] metaData;
        delete [] writeMetaData;
    }   

};
#endif