#include <string>
#ifndef FIFO_H
#define FIFO_H
using namespace std;
bool insertIntoFreeCacheFIFO(string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
int insertIntoFullCacheFIFO(string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
#endif