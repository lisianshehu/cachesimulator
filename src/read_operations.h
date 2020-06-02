#ifndef READ_OPERATIONS_H
#define READ_OPERATIONS_H
#include <string>
#include "Cache.h"
#include "LRU.h"
#include "FIFO.h"
#include "meta_data.h"
extern void perform_read(Cache *cache, string operation, int set_number, unsigned long long int tag_address);
#endif
