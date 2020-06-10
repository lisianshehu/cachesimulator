#ifndef CACHE_H
#define CACHE_H
#include <string>
using namespace std;

class Cache
{
    public:
        int num_blocks;
        int associativity;
        int write_policy;

        // data structures for cache
        unsigned long long int **cache;
        int **metaData;
        int **writeMetaData;

        struct CacheData
        {
            bool hit = false;
            int indexHit = 0;
            int readsMem = 0;
            int writesMem = 0;
            int totalMisses = 0;
        }cache_data;

        void initialize_cache();
        void initialize_metaData();
        void initialize_writeMetaData();
        int write_back_data_check(Cache *cache, int set_number, int replcIndex, unsigned long long int tag_address);

        virtual bool insert_into_free_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address) = 0;
        virtual int insert_into_full_cache(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address) = 0;
        virtual void update_meta_data(Cache *cache, int set_number) = 0;
    
    Cache(int num_blocks, int associativity, int write_policy)
    {
        this->num_blocks = num_blocks;
        this->associativity = associativity;
        this->write_policy = write_policy;
    }

    virtual ~Cache(){}
};
#endif
