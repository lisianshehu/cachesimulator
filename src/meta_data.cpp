#include "meta_data.h"
using namespace std;
// updates metadata for a read/write hit
void updateMetaData (int indexHit, int setNumber, int numberOfBlocks, int associativity, unsigned long long int **cache, int **metaData)
{
    for (int i = 0; i < associativity; i++)
    {
        if ((metaData[setNumber][i] < metaData[setNumber][indexHit]))
        {
            metaData[setNumber][i]++;
        }
    }
    metaData[setNumber][indexHit] = 0;
}


// checks for dirty bit to write back
int writeBackDataCheck (int writesMem, int setNumber, int numberOfBlocks, int associativity, int replcIndex, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    if (writeMetaData[setNumber][replcIndex] == 1)
    {
        return 1;
    }

    return 0;
}