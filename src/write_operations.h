#ifndef WRITE_OPERATIONS_H
#define WRITE_OPERATIONS_H
#include <string>
#include "Cache.h"
#include "meta_data.h"
#include "LRU.h"
#include "FIFO.h"
extern void perform_write(Cache *cache, string operation, int set_number, unsigned long long int tag_address);
#endif