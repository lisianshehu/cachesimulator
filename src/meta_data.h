#ifndef META_DATA_H
#define META_DATA_H
void updateMetaData(int indexHit, int setNumber, int numberOfBlocks, int associativity, unsigned long long int **cache, int **metaData);
int writeBackDataCheck(int writesMem, int setNumber, int numberOfBlocks, int associativity, int LRUIndex, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData);
#endif