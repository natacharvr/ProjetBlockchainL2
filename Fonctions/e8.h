#include "e7.h"

typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;

CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
//deux fonction annexes
void add_child_recursif(CellTree* child, CellTree* new);
void update_height_recursif(CellTree* cell);

void print_tree(CellTree* tree);
void delete_node(CellTree* node);
void delete_tree(CellTree* tree);

CellTree* highest_child(CellTree* tree);
CellTree* last_node(CellTree* tree);
CellProtected* fusion_lists(CellProtected* a, CellProtected* b);
CellProtected* fusion_lists_votes(CellTree* tree);
