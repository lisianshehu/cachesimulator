#ifndef FIFO_H
#define FIFO_H
#include <string>
#include "meta_data.h"
bool insertIntoFreeCacheFIFO(Cache *cache, std::string operation, int set_number, unsigned long long int tagAddress);
int insertIntoFullCacheFIFO(Cache *cache, std::string operation, int set_number, unsigned long long int tagAddress);
#endif