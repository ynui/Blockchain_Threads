#include "server.h"

void startServer()
{
	initServer();
	int res;
	pthread_t server_thread;
	res = pthread_create(&server_thread, NULL, serve, NULL);
	assert(res == 0);
	pthread_join(server_thread, NULL);
}

void *serve()
{
	while (true)
	{
		pthread_mutex_lock(&l_mission_lock);
		while (isBlockchainEmpty(missionsQueueBlocks))
			pthread_cond_wait(&g_c_mission, &l_mission_lock);
		BLOCK_T *mission = getMission();
		if (isBlockValid(mission))
		{
			destroy(missionsQueueBlocks);
			addToBlockchain(mission);
			checkAndPrintServerActionToLog(mission, true);
		}
		else
		{
			checkAndPrintServerActionToLog(mission, false);
			free(mission);
		}
		pthread_mutex_unlock(&l_mission_lock);
	}
}

BLOCK_T *getMission()
{
	Node* mission = missionsQueueBlocks->head; 
	BLOCK_T *res = mission->data;
	missionsQueueBlocks->head = missionsQueueBlocks->head->next;
	free(mission);
	return res;
}

void initServer()
{
	BLOCK_T *genesisBlock = initGenesisBlock();
	pthread_mutex_lock(&l_block_lock);
	blockchainAdd(genesisBlock, g_Blockchain);
	pthread_mutex_unlock(&l_block_lock);
	pthread_cond_broadcast(&g_c_empty);
}

void addToBlockchain(BLOCK_T *newBlock)
{
	pthread_mutex_lock(&l_block_lock);
	blockchainAdd(newBlock, g_Blockchain);
	pthread_mutex_unlock(&l_block_lock);
}

BLOCK_T *initGenesisBlock()
{
	BLOCK_T *genesisBlock = (BLOCK_T *)malloc(sizeof(BLOCK_T));
	genesisBlock->timestamp = (int)time(NULL);
	genesisBlock->height = 0;
	genesisBlock->prev_hash = 0;
	genesisBlock->difficulty = DIFFICULITY;
	genesisBlock->relayed_by = 0;
	genesisBlock->nonce = 0;
	genesisBlock->hash = generateBlockHash(genesisBlock);
	return genesisBlock;
}

void initBlockchain()
{
	g_Blockchain = makeBlockchain();
	missionsQueueBlocks = makeBlockchain();
	initGlobals(g_Blockchain,missionsQueueBlocks);
}


	

void checkAndPrintServerActionToLog(BLOCK_T *block, bool isAddedToBlockchain)
{
	if(isAddedToBlockchain)
	{
		printf("Server: block added by %d, attributes: height(%d), timestamp(%d), hash(%08x), prev_hash(%08x), difficulty(%d), nonce(%d)\n", block->relayed_by, block->height,
			block->timestamp, block->hash,
			block->prev_hash, block->difficulty,
			block->nonce);
	}
	else
	{
		checkAndPrintServerBlockRejectionReason(block);
	}
}

void checkAndPrintServerBlockRejectionReason(BLOCK_T* block)
{
	unsigned int expectedHash = generateBlockHash(block);
	if(block->hash != expectedHash)
	{
		printf("Server: wrong hash for block # %d by miner # %d, received %08x but calculated %08x\n",
		block->height, block->relayed_by,
		block->hash, expectedHash);
	} else if(!isHashDifficulityOK(block->hash))
	{
		printf("Server: wrong hash difficulity for block # %d by miner # %d, received %08x but difficulity is %d\n",
		block->height, block->relayed_by,
		block->hash, DIFFICULITY);
	} else if(block->height - 1 != g_Blockchain->head->data->height)
	{
		printf("Server: wrong height for block by miner # %d, received %d but expected %d\n",
		block->relayed_by, block->height, g_Blockchain->head->data->height + 1);
	} else
	{
		printf("Server: error edding block:\n");
		printBlock(block);
	}
}