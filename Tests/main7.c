#include "../Fonctions/e7.h"

int main(int argc, char const *argv[]) {

	// à partir d'un fichier block.txt, on calcule la
	//proof_of_work pour i entre 0 et 6 et on l'écrit dans le fichier benchmark_SHA256

	Block * b;
	clock_t t_hash1, t_hash2;
	FILE* f_bench = fopen("benchmark_SHA256", "w");
	for (int i = 0; i < 4; i++){
		b = file_to_block("../block.txt");
		//A la première lecture, on écrit dans un fichier temporaire le contenu du bloc pour vérifier sa fonctionnalité
		if (i == 0){
			block_to_file("block_test.txt", b);
			printf("Contenu du bloc :\n");
			system("cat block_test.txt");
			remove("block_test.txt");
		}

		printf("\nAvant hachage de la PoW: %s\n", b->hash);
		t_hash1 = clock();
		compute_proof_of_work(b, i);
		t_hash2 = clock();
		printf("Après hachage de la PoW: %s (nonce=%d)\n", b->hash, b->nonce);

		fprintf(f_bench, "temps passé pour %d zéros : %.5f\n", i,(double)(t_hash2-t_hash1)/CLOCKS_PER_SEC);

		delete_list_pr(b->votes);
		b->votes = NULL;
		free(b->author);

		delete_block(b);
	}
	fclose(f_bench);

    return 0;
}
