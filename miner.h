#ifndef __miner_
#define __miner_
#include "globals.h"
#define NUM_OF_MINERS 4

void minersInit();
bool mineBlock(BLOCK_T**, int);
void* mine(void*);
void* badMine(void*);
void getBlockValues(int , BLOCK_T* , BLOCK_T *);
void PrintMinerActionToLog(BLOCK_T*);
void addToBlockchain(BLOCK_T*);
unsigned int generateBlockBadHash(BLOCK_T *);
#endif // !#define __miner_
