#ifndef WRITE_OPERATIONS_H
#define WRITE_OPERATIONS_H
#include <string>
#include "Cache.h"
#include "meta_data.h"
#include "LRU.h"
#include "FIFO.h"
extern void write_operation_control(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
extern void write_hit(Cache *cache, int set_number, unsigned long long int tag_address);
extern void write_miss(Cache *cache, std::string operation, int set_number, unsigned long long int tag_address);
#endif