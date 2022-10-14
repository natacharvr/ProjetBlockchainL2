#include "e2.h"

#define TAILLE_KEY 12
#define TAILLE_PROTECTED 81

typedef struct key{
	long n;
	long val;
} Key;

typedef struct signature{
	int size;
	long* tab;
} Signature ;

typedef struct protected{
	Key* pKey;
	char* mess;
	Signature* sig;
}Protected;

void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key); //malloc str(12)
Key* str_to_key(char* str); //malloc key

Signature* init_signature(long* content, int size); //malloc signature
Signature* sign(char* mess, Key* sKey);
char* signature_to_str ( Signature* sgn ); //malloc str
Signature* str_to_signature ( char* str ); //malloc long[]

Protected* init_protected(Key* pKey, char* mess, Signature* sig); //malloc Protected
int verify(Protected* pr);
char* protected_to_str( Protected* pr ); //malloc str(81)
Protected* str_to_protected(char* str); //malloc Protected

void free_signature(Signature* sig);
void free_protected(Protected* pr);
