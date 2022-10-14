#include "../Fonctions/e4.h"

int main(){
	srand(time(NULL));
	int nb_votant = 10;
	int nb_candidats = 5;
	//génération des fichiers keys.txt, canidates.txt et declarations.txt
	generate_random_data(nb_votant, nb_candidats);
	printf("Contenu du fichier keys.txt:\n\n");
	system("cat ../keys.txt");

	printf("\n\nContenu du fichier candidates.txt:\n\n");
	system("cat ../candidates.txt");

	printf("\n\nContenu du fichier declarations.txt:\n\n");
	system("cat ../declarations.txt");
	return 1;
}
