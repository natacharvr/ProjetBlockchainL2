#include "e5.h"

typedef struct hashcell{
    Key* key; //cle publique de la personne
    int val; //nb votes comptabilisés dans la table de hachage des candidats
    //1 si a voté et 0 sinon dans la table des électeurs
} HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
} HashTable;

CellProtected* verify_list(CellProtected* liste);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
int is_key_identical(Key* k1, Key* k2);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
Key* count_winner(HashTable* t, int size);
