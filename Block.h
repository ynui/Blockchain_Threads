#ifndef _block_T
#define _block_T

typedef struct {
	int 	         height;        // Incrementeal ID of the block in the chain
	int     	     timestamp;    // Time of the mine in seconds since epoch
	unsigned int     hash;        // Current block hash value
	unsigned int     prev_hash;    // Hash value of the previous block
	int        		 difficulty;    // Amount of preceding zeros in the hash
	int         	 nonce;        // Incremental integer to change the hash value
	int         	 relayed_by;    // Miner ID
} BLOCK_T;



#endif // !_block_T