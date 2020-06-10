#ifndef READ_OPERATIONS_H
#define READ_OPERATIONS_H
#include <string>
#include "Cache.h"
#include "LRU.h"
#include "FIFO.h"
void read_operation_control(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
void read_miss(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
#endif
