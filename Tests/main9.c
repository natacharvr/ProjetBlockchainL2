#include "../Fonctions/e9.h"
#define SIZE_V 11
#define SIZE_C 5
#define D 2

int main(){

    system("rm ../Blockchain/* -f");

	//Génération des données du vote et lecture dans des listes chainées.
    generate_random_data(SIZE_V, SIZE_C);
	FILE* file = fopen("../declarations.txt", "r");
    CellProtected* declarations = read_protected(file);
	fclose(file);
	CellTree* t = NULL;

    CellKey* voters = read_public_keys("../keys.txt");
    CellKey* candidates = read_public_keys("../candidates.txt");

    CellProtected* temp = declarations;
    CellTree* temp2 = NULL;
    CellProtected* temp3 = NULL;
    int j = 1;

	//Tant que la liste des déclarations n'est pas nulle, on va créer des blocs de dix votes à soumettre à la blockchain.
    while (temp != NULL){
        for (int i = 0; i < 10; i++){
            submit_vote(temp->data);
            temp = temp->next;
            if (temp == NULL){
                i = 10;
            }
        }
		//Une fois que 10 votes ont été soumis, on les inscrit dans un bloc d'auteur le premier électeur qui sera écrit dans un fichier situé dans le dossier Blockchain.
		create_block(t, voters->data, D);
        char name[100];
        sprintf(name, "Block_%d",j);
        j++;
        add_block(D, name);
		temp2 = t;
        while (temp2 != NULL){
            temp3 = temp2->block->votes;
			free(temp2->block->author);
            while (temp3 != NULL){
                free_protected(temp3->data);
                temp3 = temp3->next;
            }
            temp2 = temp2->firstChild;
        }
        delete_tree(t);
		//On lit l'abre écrit dans les fichiers du dossier Blockchain pour pouvoir transmettre
		//à la fonction create_block un arbre constitué de tous les votes déjà soumis afin qu'il connaisse
		//la valeur hachée du dernier bloc pour l'ajouter à la fin correctement.
        t = read_tree();
    }


    printf("Arbre final : \n");
    print_tree(t);

	//On compte alors le gagnant de l'élection
    Key* winner = compute_winner_BT(t, candidates, voters, SIZE_C*2, SIZE_V*2);
    char* res = key_to_str(winner);

    printf("Le gagnant est : %s\n", res);

    free(res);
    free(winner);
	CellTree* parcours = t;
	while (parcours){
		free(parcours->block->author);
		parcours = parcours->firstChild;
	}
	delete_tree(t);
    delete_list_keys(candidates);
    delete_list_keys(voters);
	delete_list_pr(declarations);

    return 0;
}
