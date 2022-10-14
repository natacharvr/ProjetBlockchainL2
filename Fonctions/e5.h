#include "e4.h"

typedef struct cellKey{
    Key* data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected{
    Protected* data;
    struct cellProtected* next;
}CellProtected;

CellKey* create_cell_key(Key* key);
void add_key_front(CellKey** list, Key* key);
CellKey* read_public_keys(char* file);
void print_list_keys(CellKey* list);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* list);

CellProtected* create_cell_protected(Protected* pr);
void add_pr_front(CellProtected** list, Protected* pr);
CellProtected* read_protected(FILE* file);
void print_list_protected(CellProtected* list);
void delete_cell_pr(CellProtected* c);
void delete_list_pr(CellProtected* list);
