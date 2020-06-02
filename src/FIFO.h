#ifndef FIFO_H
#define FIFO_H
#include <string>
#include "meta_data.h"
bool insertIntoFreeCacheFIFO(Cache *cache, string operation, int set_number, unsigned long long int tagAddress);
int insertIntoFullCacheFIFO(Cache *cache, string operation, int set_number, unsigned long long int tagAddress);
#endif