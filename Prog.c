#include "Prog.h"

int main()
{
	initMutexes();
	initBlockchain();
	minersInit();
	startServer();
}