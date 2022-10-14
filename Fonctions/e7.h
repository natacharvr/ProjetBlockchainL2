#include "e6.h"
#include <openssl/sha.h>

typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

void block_to_file(char* fileName, Block* b);
Block* file_to_block(char* fileName);
char* block_to_str(Block* block);
void delete_block(Block* b);
unsigned char* hash_str(unsigned char* str);
void compute_proof_of_work(Block* b, int d);
int verify_block(Block* b, int d);
int has_n_zeros(unsigned char* str, int n);
