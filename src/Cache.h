#ifndef CACHE_H
#define CACHE_H

class Cache
{
    public:
        int num_blocks;
        int associativity;
        int replacementPolicy;
        int writePolicy;
        unsigned long long int **cache;
        int **metaData;
        int **writeMetaData;

        struct CacheData
        {
            bool hit;
            int totalHits;
            int indexHit;
            int readsMem;
            int writesMem;
            int totalMisses;
        } cache_data;

        void initialize_cache();
        void initialize_metaData();
        void initialize_writeMetaData();

    Cache(int num_blocks, int associativity, int replacementPolicy, int writePolicy)
    {
        this->num_blocks = num_blocks;
        this->associativity = associativity;
        this->replacementPolicy = replacementPolicy;
        this->writePolicy = writePolicy;
    }
   
};
#endif
