#ifndef FIFO_H
#define FIFO_H
#include <string>
#include "meta_data.h"

class FIFOCache : public Cache
{
    public:
        bool insert_into_free_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
        int insert_into_full_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
        void updateMetaData(Cache *cache, int set_number);
    FIFOCache(int num_blocks, int associativity, int write_policy) : Cache(num_blocks, associativity, write_policy){};
};
#endif