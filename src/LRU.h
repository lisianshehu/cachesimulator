#include <string>
#ifndef LRU_H
#define LRU_H
using namespace std;
bool insertIntoFreeCacheLRU(string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
int insertIntoFullCacheLRU(string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
#endif