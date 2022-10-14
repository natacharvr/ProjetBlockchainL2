#include "e5.h"

CellKey* create_cell_key(Key* key){
	//Crée une cellule de liste chainée de clés
    CellKey* cell = malloc(sizeof(CellKey));
	if (!cell){
		printf("Erreur d'allocation (create_cell_key)\n");
		return NULL;
	}

    cell->data = key;
    cell->next = NULL;
    return cell;
}

void add_key_front(CellKey** list, Key* key){
	//Ajoute une cellule de CellKey au début d'une liste chaînée
    CellKey* cell = create_cell_key(key);
    cell->next = *list;
    *list = cell;
}

CellKey* read_public_keys(char* nameFile){
	//lit les clés publiques dans un fichier sur lequel la clé est en première position sur la ligne
	//read_public_keys les range dans une liste chainée grâce à add_key_front
    FILE* file = fopen(nameFile, "r");
    if(file){
        long val1, n1;
        char buffer[20];
        CellKey* list = NULL;
        Key* key;

		//tant que l'on atteint pas la fin du fichier on continue à lire
        while (!feof(file)){
            fscanf(file, "(%lx,%lx)%s\n", &val1, &n1, buffer);
            key = malloc(sizeof(Key));
			if (!key){
				printf("Erreur d'allocation (read_public_keys)\n");
				return NULL;
			}

            init_key(key, val1, n1);
            add_key_front(&list, key);
        }
        fclose(file);
        return list;
    }
    return NULL;
}

void print_list_keys(CellKey* list){
	//Affiche une liste chaînée de clés
    CellKey* temp = list;
    if (temp){
        while (temp->next){
            printf("(%lx,%lx)\n", temp->data->val, temp->data->n);
            temp = temp->next;
        }
    }
    printf("(%lx,%lx)\n", temp->data->val, temp->data->n);
}

void delete_cell_key(CellKey* c){
	//supprime la mémoire allouée par une cellule de CellKey
    free(c->data);
    free(c);
}

void delete_list_keys(CellKey* list){
	//Supprime la mémoire allouée par une liste chaînée de clés
    CellKey* temp;
    while(list){
        temp = list->next;
        delete_cell_key(list);
        list = temp;
    }
}

CellProtected* create_cell_protected(Protected* pr){
	//Crée une cellule de liste chainée de protected
    CellProtected* cell = malloc(sizeof(CellProtected));
	if (!cell){
		printf("Erreur d'allocation (create_cell_protected)\n");
		return NULL;
	}

    cell->data = pr;
    cell->next = NULL;
    return cell;
}

void add_pr_front(CellProtected** list, Protected* pr){
	//Ajoute une cellule de CellProtected au début d'une liste chaînée
    CellProtected* cell = create_cell_protected(pr);
    cell->next = *list;
    *list = cell;
}

CellProtected* read_protected(FILE * file){
	//lit les déclarations dans un fichier
	//read_protected les range dans une liste chainée grâce à add_pr_front
    if(file){
        CellProtected* list = NULL;
        Protected* pr;
        char str[200], str2[30], str3[300];

		//tant que l'on atteint pas la fin du fichier on continue à lire
        while(!feof(file)){
            fscanf(file, "%s %s %s\n", str, str2, str3);
            strcat(strcat(strcat(strcat(str, " "), str2)," "), str3);
            pr = str_to_protected(str);
            add_pr_front(&list, pr);
        }
        return list;
    }
    return NULL;
}

void print_list_protected(CellProtected* list){
	//Affiche une liste chainée de déclarations
    if (list == NULL) return;
    CellProtected* temp = list;
    char* str;
    if (temp){
        while (temp->next){
            str = protected_to_str(temp->data);
            printf("%s\n", str);
            free(str);
            temp = temp->next;
        }
    }
    str = protected_to_str(temp->data);
    printf("%s\n", str);
    free(str);
}

void delete_cell_pr(CellProtected* c){
	//Supprime la mémoire allouée dans une cellule de liste chainée de protected
    free_protected(c->data);
    free(c);
}

void delete_list_pr(CellProtected* list){
	//supprime la mémoire allouée dans une liste chainée de protected
    CellProtected* temp;
    while(list){
        temp = list->next;
        delete_cell_pr(list);
        list = temp;
    }
}
