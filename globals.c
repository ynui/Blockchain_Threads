#ifndef __globals_
#define __globals_

#include "globals.h"

void initMutexes()
{
	pthread_mutex_init(&l_block_lock,NULL);
	pthread_mutex_init(&l_mission_lock,NULL);
	pthread_cond_init(&g_c_empty,NULL);
	pthread_cond_init(&g_c_mission,NULL);
}

BLOCK_T* getLastBlock()
{
	BLOCK_T* ret;
	pthread_mutex_lock(&l_block_lock);
	while(isBlockchainEmpty(g_Blockchain))
		pthread_cond_wait(&g_c_empty,&l_block_lock);
	ret = g_Blockchain->head->data;
	pthread_mutex_unlock(&l_block_lock);
	return ret;
}

void addMissionBlock(BLOCK_T* block)
{
	Node* newNode = createnode(block);
	pthread_mutex_lock(&l_mission_lock);
	addToTail(newNode,missionsQueueBlocks);
	pthread_mutex_unlock(&l_mission_lock);
}

void initGlobals(Blockchain* blockchain, Blockchain* missionsQueue)
{
	g_Blockchain = blockchain;
	missionsQueueBlocks = missionsQueue;
}

bool isBlockValid(BLOCK_T *minedBlock)
{
	unsigned int calculatedCrc32Hash = generateBlockHash(minedBlock);
	return (calculatedCrc32Hash == minedBlock->hash) &&
		   (isBlockLegal(minedBlock));
}
bool isHashDifficulityOK(unsigned int hash)
{
	if(DIFFICULITY == 0) return true;
	unsigned int mask = 0xFFFFFFFF;
	mask <<= (32 - DIFFICULITY);
	return ((hash & mask) == 0);
}

bool isBlockLegal(BLOCK_T* block)// check hash and height
{
	
	return isHashDifficulityOK(block->hash) &&
			block->height - 1 == g_Blockchain->head->data->height;
}

void copy_block(BLOCK_T* src, BLOCK_T** dest)
{
	*dest = (BLOCK_T*)malloc(sizeof(BLOCK_T));
	(*dest)->height = src->height;
	(*dest)->timestamp = src->timestamp;
	(*dest)->hash = src->hash;
	(*dest)->prev_hash = src->prev_hash;
	(*dest)->difficulty = src->difficulty;
	(*dest)->nonce = src->nonce;
	(*dest)->relayed_by = src->relayed_by;
}

unsigned int generateBlockHash(BLOCK_T *block)
{
	char toHash[250];
	snprintf(toHash, sizeof(toHash), "%u%d%d%d%d%d",
			 block->prev_hash, block->height,
			 block->difficulty, block->relayed_by,
			 block->timestamp, block->nonce);
	return crc32(0, toHash, stringlen(toHash));
}

int stringlen(char *str)
{
    int len = 0;
    int i;
    for (i=0; str[i] != 0; i++) 
    {
        len++;
    }
    return(len);
}

#endif //!__globals_