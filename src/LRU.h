#ifndef LRU_H
#define LRU_H
#include <string>
#include "Cache.h"
#include "meta_data.h"

class LRUCache : public Cache
{
    public: 
        void updateMetaData(Cache *cache, int set_number);
        bool insert_into_free_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
        int insert_into_full_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
        
    LRUCache(int num_blocks, int associativity, int write_policy) : Cache(num_blocks, associativity, write_policy){};

    ~LRUCache()
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