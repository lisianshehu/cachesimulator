#ifndef LRU_H
#define LRU_H
#include <string>
#include "Cache.h"
#include "meta_data.h"
using namespace std;
extern bool insertIntoFreeCacheLRU(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
extern int insertIntoFullCacheLRU(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
#endif