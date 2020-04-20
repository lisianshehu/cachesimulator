#ifndef CACHE_H
#define CACHE_H

class Cache
{
    public:
        int num_blocks;
        int associativity;
        unsigned long long int **cache;
        int **metaData;
        int **writeMetaData;

        void initialize_cache();
        void initialize_metaData();
        void initialize_writeMetaData();

    Cache(int num_blocks, int associativity)
    {
        this->num_blocks = num_blocks;
        this->associativity = associativity;
    }
   
};
#endif
