#include "e4.h"

void generate_random_data(int nv, int nc){
    if (nc > nv) nc = nv; //si le nombre de candidats demandés est supérieur au nombre de votants, le nombre de candidats est changé au nombre de votants.
    //génération des clefs
    FILE* f_keys = fopen("../keys.txt", "w");
    if (f_keys == NULL){
        printf("erreur d'ouverture du fichier\n");
        return;
    }
    Key* pKey = malloc(sizeof (Key));
    Key* sKey = malloc(sizeof (Key));

    if ((pKey == NULL) || (sKey == NULL)) {
        printf("Erreur d'allocation (generate_random_data)\n");
        return;
    }

    char* str_pkey;
    char* str_skey;

	//génération des clés des électeurs et écriture dans le fichier keys.txt
    for (long i = 0; i < nv; i++){
        init_pair_keys(pKey , sKey ,3 ,7);
        str_pkey = key_to_str(pKey);
        str_skey = key_to_str(sKey);
        fprintf(f_keys, "%s %s\n", str_pkey, str_skey);
        free(str_pkey);
        free(str_skey);
    }
    free(pKey);
    free(sKey);
    fclose(f_keys);

    //génération des candidats
    FILE* f_keys_read = fopen("../keys.txt", "r");
    FILE* f_candidates = fopen("../candidates.txt", "w");
    if ((f_keys_read==NULL)||(f_candidates==NULL)){
        printf("erreur d'ouverture du fichier (generate_random_data)\n");
        return;
    }

	//On crée un tableau pour conserver les numéros des lignes déja utilisées afin de ne pas faire candidater deux fois un même citoyen
    int* used = malloc(sizeof(int) * nc);
	if (!used){
		printf("Erreur d'allocation (generate_random_data)\n");
		return	;
	}
    for (int j = 0; j < nc; j++) used[j] = 0;
    char public[50];
    char secret[50];
    int r;

	//On sélectionne aléatoirement des lignes du fichier et on récupère la clé publique des citoyen sélectionnés que l'on écrit dans le fichier candidates.txt
    for (int i = 0; i < nc; i++){
        rewind(f_keys_read);
        do {
            r = rand() % nv + 1;
        } while (is_in(r, used, nc));
        used[i] = r;
        for (int j = 0; j < r; j++){
            fscanf(f_keys_read, "%s %s\n", public, secret);
        }
        fprintf(f_candidates, "%s\n", public);
    }
    fclose(f_candidates);

    //Simulation du vote
    char str_key_candidate[50];
    char* declaration;
    Key* key_vote_secret;
    Key* key_vote_public;
    Signature* sig;
    Protected* pr;
    rewind(f_keys_read);
    FILE* f_candidates_read = fopen("../candidates.txt", "r");
    FILE* f_votes = fopen("../declarations.txt", "w");

    if ((f_candidates_read == NULL) || (f_votes == NULL)){
        printf("erreur d'ouverture du fichier\n");
        return;
    }

    for(int k = 0; k < nv; k++){
        //on récupère les clefs du citoyen qui vote
        fscanf(f_keys_read, "%s %s\n", public, secret);
        //on sélectionne un candidat au hasard
        r = rand() % nc + 1;
        for (int j = 0; j < r; j++){
            fscanf(f_candidates_read, "%s\n", str_key_candidate);
        }
        rewind(f_candidates_read);

        //On génère la signature
        key_vote_secret = str_to_key(secret);
        key_vote_public = str_to_key(public);

        sig = sign(str_key_candidate, key_vote_secret);

		//on crée et écrit cette déclaration de vote dans le fichier declarations.txt
        pr = init_protected(key_vote_public, str_key_candidate, sig);
        if (!verify(pr)) {
            printf ("Signature non valide\n");
        } else {
            declaration = protected_to_str(pr);
            fprintf(f_votes, "%s\n", declaration);
            free(declaration);
        }

        free(key_vote_secret);
        free_protected(pr);

    }
    free(used);
    fclose(f_votes);
    fclose(f_candidates_read);
    fclose(f_keys_read);

}

int is_in(int r, int* tab, int size){
	//Renvoie 1 si un entier r existe dans le tableau tab
    for (int i = 0; i < size; i++){
        if (tab[i] == r) return 1;
    }
    return 0;
}
