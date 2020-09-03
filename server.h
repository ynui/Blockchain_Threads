#ifndef __server_
#define __server_
#include "Blockchain.h"
#include "globals.h"
Blockchain *g_Blockchain, *missionsQueueBlocks;

void* serve();
void startServer();
void initServer();
void addToBlockchain(BLOCK_T *);
unsigned int generateBlockHash(BLOCK_T*);
BLOCK_T* initGenesisBlock();
BLOCK_T* getMission();
void checkAndPrintServerActionToLog(BLOCK_T* ,bool);
void checkAndPrintServerBlockRejectionReason(BLOCK_T*);
 #endif // !__serverr