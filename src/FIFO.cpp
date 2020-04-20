#include "FIFO.h"
#include "meta_data.h"
using namespace std;

// inserts into cache using FIFO replacement with a full set
int insertIntoFullCacheFIFO(string operation, int writePolicy, int writesMem, int setNumber, int numberOfBlocks, int associativity, int maxPosition, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    int FIFOIndex = 0;
    int writeBack = 0;
    for (int i = 0; i < associativity; i++)
    {
        if (metaData[setNumber][i] == 1)
        {
            FIFOIndex = i;
        }
    }
    cache[setNumber][FIFOIndex] = tagAddress;
    for (int i = 0; i < associativity; i++)
    {
        if (metaData[setNumber][i] > 1)
        {
            metaData[setNumber][i]--;
        }
    }

    // write-back algorithim for writing to memory if there is a dirty bit in place
    if (writePolicy == 1)
    {
        writeBack = writeBackDataCheck(writesMem, setNumber, numberOfBlocks, associativity, FIFOIndex, tagAddress, cache, metaData, writeMetaData);
        if (operation == "R")
        {
            writeMetaData[setNumber][FIFOIndex] = 0; // sets dirty bit to 0 for a read-miss when new cache block is allocated
        }
        else if (operation == "W")
        {
            writeMetaData[setNumber][FIFOIndex] = 1; // sets dirty bit to 1 for a write-miss when new cache block is allocated
        }
    }
    metaData[setNumber][FIFOIndex] = associativity;
    return writeBack;
}

bool insertIntoFreeCacheFIFO(string operation, int writePolicy, bool setFull, int setNumber, int numberOfBlocks, int associativity, unsigned long long int tagAddress, unsigned long long int **cache, int **metaData, int **writeMetaData)
{
    if (cache[setNumber][associativity-1] != -1)
    {
        return true;
    }
    else 
    {
        for (int i = 0; i < associativity; i++)
        {
            if (cache[setNumber][i] == -1)
            {
                cache[setNumber][i] = tagAddress;
                int newDataIndex = i;
                // setFull = false;
                metaData[setNumber][i] = newDataIndex+1;
                if (operation == "W" && writePolicy == 1)
                {
                    writeMetaData[setNumber][newDataIndex] = 1;
                } 
                break;
            } 
        }
    }
    return false;
}