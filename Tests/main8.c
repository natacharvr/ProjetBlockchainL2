#include "../Fonctions/e8.h"

int main(){
	//Lecture du premier bloc et création d'un noeud le contenant
    Block* b = file_to_block("../block.txt");
    CellTree* racine = create_node(b);

	//Lecture du deuxième bloc et création d'un noeud le contenant
    Block* b2 = file_to_block("../block1.txt");
    CellTree* fils = create_node(b2);

	//Ajout du deuxième bloc à l'arbre
    int h = update_height(racine, fils);
    add_child(racine, fils);

	printf("Affichage de l'arbre constitué de deux blocs :\n");
    print_tree(racine);

    printf("h = %d\nracine height = %d\nfils height = %d\n\n", h, racine->height, fils->height);

	//Test des fonctions highest_child et last_node
    CellTree * highestchild = highest_child(racine);
	printf("Le plus grand fils de la racine est :\n");
    print_tree(highestchild);
    CellTree * lastnode = last_node(racine);
	printf("Le last node de l'arbre est :\n");
    print_tree(lastnode);

	//Test de la fonction fusion_lists_votes
	printf("Les votes contenus dans tout l'arbre sont : \n");
    CellProtected * votes = fusion_lists_votes(racine);
    print_list_protected(votes);
    delete_list_pr(votes);

    free(b->author);
    free(b2->author);
    delete_tree(racine);

    return 0;
}
