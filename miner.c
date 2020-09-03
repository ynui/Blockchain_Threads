#include "miner.h"

void minersInit()
{
	int res;
	pthread_t miner_threads[NUM_OF_MINERS];
	pthread_t minerFucker;
	int i;
	for (i = 1; i < NUM_OF_MINERS + 1; i++)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		res = pthread_create(&miner_threads[i], NULL, mine, arg);
		assert(res == 0);
	}
	if(DIFFICULITY != 0)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		res = pthread_create(&minerFucker, NULL, badMine, arg);
		assert(res == 0);
	}
}

void* mine(void* minerID)
{
	BLOCK_T minedBlock;
	int ID = *((int *) minerID);
	free(minerID);
	while(true)
	{
		BLOCK_T* lastBlockFromChain = getLastBlock();
		getBlockValues(ID, lastBlockFromChain,&minedBlock);
		do {
			minedBlock.nonce++;
			minedBlock.timestamp = (int)time(NULL);
			minedBlock.hash = generateBlockHash(&minedBlock);
			if(isBlockLegal(&minedBlock))
			{
				BLOCK_T *newBlock;
				copy_block(&minedBlock, &newBlock);
				addMissionBlock(newBlock);
				PrintMinerActionToLog(newBlock);
				pthread_cond_signal(&g_c_mission);
			}
		}while(lastBlockFromChain == getLastBlock());
	}
}

void* badMine(void* minerID)
{
	BLOCK_T minedBlock;
	int ID = *((int *) minerID);
	free(minerID);
	while(true)
	{
		sleep(1);
		BLOCK_T* lastBlockFromChain = getLastBlock();
		getBlockValues(ID, lastBlockFromChain,&minedBlock);
		minedBlock.nonce++;
		minedBlock.timestamp = (int)time(NULL);
		minedBlock.hash = generateBlockBadHash(&minedBlock);
		BLOCK_T *newBlock;
		copy_block(&minedBlock, &newBlock);
		addMissionBlock(newBlock);
		PrintMinerActionToLog(newBlock);
		pthread_cond_signal(&g_c_mission);
	}
}

void getBlockValues(int minerID, BLOCK_T* lastBlockFromChain, BLOCK_T *res)
{
	res->height = lastBlockFromChain->height + 1;
	res->prev_hash = lastBlockFromChain->hash;
	res->difficulty = DIFFICULITY;
	res->relayed_by = minerID;
	res->nonce = -1;
	res->timestamp = 0;
	res->hash = 0;
}

void PrintMinerActionToLog(BLOCK_T* block)
{
	printf("Miner # %d : Mined a new block # %d, with the hash %x\n", block->relayed_by, block->height, block->hash);
}

/*
	calculates hash in a different order than the server
*/
unsigned int generateBlockBadHash(BLOCK_T *block)
{
	char toHash[250];
	snprintf(toHash, sizeof(toHash), "%d%d%d%d%d%u",
			 block->height, block->difficulty,
			 block->relayed_by, block->timestamp,
			 block->nonce, block->prev_hash);
	return crc32(0, toHash, stringlen(toHash));
}