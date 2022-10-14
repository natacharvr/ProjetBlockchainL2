#include "e6.h"

CellProtected* verify_list(CellProtected* list){
	//Vérifie que les déclarations écrites dans une liste chainée sont valides. Si elles ne le sont pas, elles sont supprimées.
	//Une liste chainée valide est retournée
    if (!list) return NULL;
    CellProtected* precedent = list;
    CellProtected* current = list->next;
    while (current){
        if (!verify(current->data)){
            precedent->next = current->next;
            delete_cell_pr(current);
            current = precedent->next;
        } else {
            precedent = current;
            current = current->next;
        }
    }
	//On gère le cas de la première déclaration de la liste à la fin car elle agit différemment comme elle n'a pas de prédécésseur
    if (!verify(list->data)){
        current = list->next;
        delete_cell_pr(list);
        list = current;
    }
    return list;
}

HashCell* create_hashcell(Key* key){
	//Création d'une cellule de table de hachage
    HashCell* cell = malloc(sizeof(HashCell));
	if (!cell){
		printf("Erreur d'allocation (create_hashcell)\n");
		return NULL;
	}

    cell->key = malloc(sizeof(Key));
	if (!cell->key){
		printf("Erreur d'allocation (create_hashcell)\n");
		return NULL;
	}

    init_key(cell->key, key->val, key->n);
    cell->val = 0;
    return cell;
}

int hash_function(Key* key, int size){
	//Renvoie la valeur hachée correspondant à une clé
    int pos = (key->val + key->n) % size;
    return pos;
}

int find_position(HashTable* t, Key* key){
	//Renvoie la position où une clé doit être rangée dans une table de hachage
	//Si la clé est déjà présente dans la table, sa position est retournée.
	//Autrement, la fonction parcours la table depuis la case pos (correspondant à la valeur hachée de la clé) jusqu'à
	//la première case vide et renvoie son indice.
    int pos = hash_function(key, t->size);

    for (int i = 0; i < t->size; i++){
        if (t->tab[(pos + i) % t->size] == NULL) return (pos + i) % t->size;
        if (is_key_identical(t->tab[(pos + i) % t->size]->key, key)){
            return (pos + i) % t->size;
        }
    }
    printf("Plus de place, et personne non présente\n");
    return -1;
}

int is_key_identical(Key* k1, Key* k2){
	//Compare deux clés, utile pour la fonction find_position
    return (k1->n == k2->n) && (k1->val == k2->val);
}

HashTable* create_hashtable(CellKey* keys, int size){
	//crée une table de hachage à partir d'une liste de clé.
	//Elle range les clés dans la table grâce à la fonction find_position
	//L'argument size doit être supérieur ou égal à la taille de la liste keys
    HashTable* table = malloc(sizeof(HashTable));
	if (!table){
		printf("Erreur d'allocation (create_hashtable)\n");
		return NULL;
	}

    table->size = size;
    table->tab = malloc(sizeof(HashCell*) * size);
	if (!table->tab){
		printf("Erreur d'allocation (create_hashtable)\n");
		return NULL;
	}

    for (int i = 0; i < size; i++){
        table->tab[i] = NULL;
    }
    int pos;

    while (keys){
        pos = find_position(table, keys->data);

        if (table->tab[pos] == NULL){
            table->tab[pos] = create_hashcell(keys->data);
        }
        keys = keys->next;
    }
    return table;
}

void delete_hashtable(HashTable* t){
	//Supprime la mémoire utilisée par une table de hachage
    for (int i = 0; i < t->size; i ++){
        if (t->tab[i] != NULL) {
            free(t->tab[i]->key);
            free(t->tab[i]);
        }
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
	//comptabilise les votes d'une liste chainée en s'assurant que chacun ne vote qu'une fois
    HashTable* hash_candidates = create_hashtable(candidates, sizeC);
    HashTable* hash_voters = create_hashtable(voters, sizeV);
    int posV, posC;
    Key* key_candidate;
	//on commence par vérifier que la personne est inscrite dans les listes électorales (c'est à dire est dans hash_voters)

    while(decl){
        posV = find_position(hash_voters, decl->data->pKey);

        if ((posV != -1) && (hash_voters->tab[posV] != NULL) &&(hash_voters->tab[posV]->val == 0)){
            hash_voters->tab[posV]->val = 1; //la personne est dans les listes et n'a pas voté, on comptabilise alors son vote
                //On vérifie alors que la personne pour laquelle elle vote est candidat
                key_candidate = str_to_key(decl->data->mess);
                posC = find_position(hash_candidates, key_candidate);
                if ((posC != -1) && (hash_candidates->tab[posC]!=NULL)){
                    //si la personne pour laquelle elle vote est candidat, on augmente son compte de voix
                    hash_candidates->tab[posC]->val++;
                }
                free(key_candidate);
        }
        decl = decl->next;
    }
    Key* winner_key = count_winner(hash_candidates, sizeC);
    delete_hashtable(hash_candidates);
    delete_hashtable(hash_voters);
    return winner_key;
}

Key* count_winner(HashTable* t, int size){
	//Détermine le gagnant d'une élection grâce à une table de hachage.
	//Si deux candidats ont le même nombre de voix, le premier présent dans la table de hachage l'emporte par défaut
    HashCell* winner = t->tab[0];
    int premier = 1;
    while (winner == NULL){
        winner = t->tab[premier];
        premier++;
    }
    for (int i = premier; i < size; i++){
        if ((t->tab[i] != NULL) && (t->tab[i]->val > winner->val)){
            winner = t->tab[i];
        }
    }
    printf("Le gagnant a %d voix\n", winner->val);
    Key* winner_key = malloc(sizeof(Key));
	if (!winner_key){
		printf("Erreur d'allocation (count_winner)\n");
		return NULL;
	}

    init_key(winner_key, winner->key->val, winner->key->n);
    return winner_key;
}
