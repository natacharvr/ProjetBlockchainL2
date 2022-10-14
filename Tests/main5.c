#include "../Fonctions/e5.h"

int main(){
	srand(time(NULL));
    int nc = 5;
    int nv = 10;
	generate_random_data(nv, nc);

	//Lecture des fichiers générés et affichage
    CellKey* candidates = read_public_keys("../candidates.txt");
    CellKey* voters = read_public_keys("../keys.txt");

	printf("Liste des clés de candidats lues : \n");
	print_list_keys(candidates);
	printf("\n");

	printf("Liste des clés de votants lues : \n");
	print_list_keys(voters);
	printf("\n");

	//Lecture des déclarations de votes dans une liste de protected et affichage
    FILE* file = fopen("../declarations.txt", "r");
    CellProtected* liste2 = read_protected(file);
    fclose(file);

	printf("Liste des protected lus : \n");
    print_list_protected(liste2);

	delete_list_keys(candidates);
    delete_list_keys(voters);
	delete_list_pr(liste2);

	return 1;
}
