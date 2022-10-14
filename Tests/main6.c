#include "../Fonctions/e6.h"

int main(){
	srand(time(NULL));
    int nc = 5;
    int nv = 1100;
	generate_random_data(nv, nc);
    CellKey* candidates = read_public_keys("../candidates.txt");
    CellKey* voters = read_public_keys("../keys.txt");

    print_list_keys(candidates);
    FILE* file = fopen("../declarations.txt", "r");
    CellProtected* liste2 = read_protected(file);
    fclose(file);

	printf("Liste lue dans le fichier:\n");
    print_list_protected(liste2);
    CellProtected* decl = verify_list(liste2);
	printf("\n Après vérification des déclarations, la liste est constituée des déclarations suivantes :\n");
    print_list_protected(decl);

	//Comptage des votes en utilisant une table de hachage dans compute_winner
    Key* winner_key = compute_winner(decl, candidates, voters, 2*nc, 2*nv);
    char* winner_str = key_to_str(winner_key);
    printf("Le gagnant est : %s\n", winner_str);
    free(winner_str);
    free(winner_key);

    delete_list_keys(candidates);
    delete_list_keys(voters);
    delete_list_pr(decl);
}
