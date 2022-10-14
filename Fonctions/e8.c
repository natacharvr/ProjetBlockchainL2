#include "e8.h"

CellTree* create_node(Block* b){
	//crée un noeud d'un arbre contenant le bloc b
    CellTree* res = malloc(sizeof(CellTree));
    if (!res) {
		printf("Erreur d'allocation (create_node)\n");
		return NULL;
	}

    res->block = b;
    res->height = 0;
    res->father = NULL;
    res->firstChild = NULL;
    res-> nextBro = NULL;
    return res;
}

int update_height(CellTree* father, CellTree* child){
	//Met à jour la hauteur de tous les ascendants d'un bloc qu'on ajoute à un arbre
    if ((child->height + 1) > father->height){
        father->height = child->height + 1;
        return 1;
    }
    return 0;
}

void update_height_recursif(CellTree* cell){
    //fonction annexe qui augmente récursivment les hauteurs des noeuds lors de l'ajout d'un fils
    if (cell->father != NULL){
        update_height(cell->father, cell);
        update_height_recursif(cell->father);
    }
}
void add_child(CellTree* father, CellTree* child){
	//Ajoute un noeud à un arbre
    child->father = father;
    if (father){
        if(father->firstChild == NULL) father->firstChild = child;
        else add_child_recursif(father->firstChild, child);
        update_height_recursif(child);
    }
}

void add_child_recursif(CellTree* child, CellTree* new){
    //fonction annexe qui parcours récursivement la liste chaînée des fils d'un noeud jusqu'à un NULL pour ajouter new
    if (child->nextBro == NULL){
        child = new;
    } else {
        add_child_recursif(child->nextBro, new);
    }
}

void print_tree(CellTree* tree){
	//Affiche un arbre
    if (!tree){
        printf("erreur : tree est nul\n");
        return;
    }
    printf("height : %d \t", tree->height);
    if (tree->block->hash) printf("%s\n", tree->block->hash);
    putchar ('\n');

    if(tree->nextBro){
        print_tree(tree->nextBro);
    }
    if(tree->firstChild){
        print_tree(tree->firstChild);
    }
}

void delete_node(CellTree* node){
	//Supprime la mémoire allouée dans un noeud
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree* tree){
	//libère la mémoire allouée dans un arbre
	if (tree){
        delete_tree(tree->nextBro);
        delete_tree(tree->firstChild);
        delete_node(tree);
    }
}

CellTree* highest_child(CellTree* tree){
	//Renvoie le fils d'un noeud ayant la plus grande valeur de hauteur (sert à récupérer la plus longue chaine d'un arbre pour récupérer une blockchain fiable)
    CellTree* res = tree->firstChild;
    CellTree* temp = NULL;
    if(res){
        temp = res->nextBro;
    }
    while (temp){
        if (temp->height > res->height){
            res = temp;
        }
        temp = temp->nextBro;
    }
    return res;
}

CellTree* last_node(CellTree* tree){
	//Renvoie le dernier noeud de la chaine la plus longue d'un arbre
    if (!tree) return NULL;
    CellTree* res = tree;
    while(res->height != 0){
        res = highest_child(res);
    }
    return res;
}

CellProtected* fusion_lists(CellProtected* a, CellProtected* b){
    //fusionne deux listes
    CellProtected* res = NULL;
    while (a){
        add_pr_front(&res, a->data);
        a = a->next;
    }
    while (b){
        add_pr_front(&res, b->data);
        b = b->next;
    }
    return res;
}

CellProtected* fusion_lists_votes(CellTree* tree){
	//fusionne toutes les listes de vote d'un arbre
    if (!tree) return NULL;
    CellProtected* res = tree->block->votes;
    CellTree* child = tree;
    while(child->height != 0){
        child = highest_child(child);
        res = fusion_lists(child->block->votes, res);
    }
    return res;
}
