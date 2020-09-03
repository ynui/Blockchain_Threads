#ifndef __locks_
#define __locks_

#include <pthread.h>
#include "Blockchain.h"

Blockchain *g_Blockchain, *missionsQueueBlocks;

pthread_mutex_t l_block_lock, l_mission_lock;
pthread_cond_t g_c_empty, g_c_mission;


void initGlobals(Blockchain*, Blockchain*);
void addMissionBlock(BLOCK_T*);
BLOCK_T* getLastBlock();
bool isBlockValid(BLOCK_T*);
bool isHashDifficulityOK(unsigned int);
bool isBlockLegal(BLOCK_T*);
void copy_block(BLOCK_T* , BLOCK_T** );
unsigned int generateBlockHash(BLOCK_T *);
int stringlen(char *);


#endif