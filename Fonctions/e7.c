#include "e7.h"

void block_to_file(char* fileName, Block* b){
    /*Fonction qui écrit dans un fichier les données d'un block
    Elle écrit les données dans cet ordre :
    author,
    hash (valeur hachée du bloc),
    previous_hash (valeur hachée du bloc précédent),
    nonce (preuve de travail),
    votes (liste de déclaration, une par ligne)*/
    FILE* block_file = fopen(fileName, "w");
    if (!block_file){
        printf("erreur d'allocation block_to_file");
        return;
    }

    //écriture de author, hash, previous_hash et nonce (sur 4 lignes)
    char* author = key_to_str(b->author);
    fprintf(block_file, "%s\n%s\n%s\n%d\n", author, b->hash, b->previous_hash, b->nonce);
	free(author);

    //écriture de la liste de déclarations (une par ligne);
    CellProtected* votes = b->votes;
    char* decl;
    while (votes){
        decl = protected_to_str(votes->data);
        fprintf(block_file, "%s\n", decl);
        free(decl);
        votes = votes->next;
    }
    fclose(block_file);
}

Block* file_to_block(char* fileName){
	//lis un block dans un fichier
    FILE* block_file_read = fopen(fileName, "r");
    if (!block_file_read) return NULL;

    Block* res = malloc(sizeof(Block));
    if (!res){
        printf("erreur d'allocation (block_to_file)");
        return NULL;
    }

    //récupération de l'auteur
    char author[TAILLE_KEY];
    fscanf(block_file_read, "%s\n", author);
    res->author = str_to_key(author);

    //récupération des valeurs hachées
	res->hash = malloc(sizeof(unsigned char) * 100);
	res->previous_hash = malloc(sizeof(unsigned char) * 100);
	if ((!res->hash) || (!res->previous_hash)){
		printf("Erreur d'allocation (file_to_block)\n");
		return NULL;
	}

	fscanf(block_file_read, "%s\n", res->hash);
	fscanf(block_file_read, "%s\n", res->previous_hash);
    if (strcmp("(null)", (char*)res->previous_hash) == 0){
        free(res->previous_hash);
		res->previous_hash = NULL;
    }

    //récupération de la preuve de travail
    fscanf(block_file_read, "%d\n", &res->nonce);
    //récupération de la liste des déclarations
    res->votes = read_protected(block_file_read);
    fclose(block_file_read);
    return res;
}

char* block_to_str(Block* block){
	//écrit un bloc dans une chaine de caractères
    char* author = key_to_str(block->author);
    char* previous_hash;
    if (block->previous_hash){
        previous_hash = (char*) block->previous_hash;
    } else {
        previous_hash = "";
    }
    char nonce[10];
    sprintf(nonce, "%d", block->nonce);

    //je compte la taille de la liste chainée votes pour savoir quelle taille allouer pour la chaine resultat
    int i = 0;
    CellProtected* votes = block->votes;
    while (votes){
        i++;
        votes = votes->next;
    }
    //la taille de la chaine est la somme des tailles des éléments qui la constituent ainsi que les séparateurs (arbitrairement $)
    int tailleChaine = i * (TAILLE_PROTECTED + 1) + TAILLE_KEY + strlen(nonce) + strlen(previous_hash) + 3;
    char* res = malloc(sizeof(char) * tailleChaine);
	if (!res){
		printf("Erreur d'allocation (block_to_str)\n");
		return NULL;
	}
    //On écrit les différentes données dans la chaine résultat
    sprintf(res, "%s", author);
    free(author);
    strcat(res, "$");
    strcat(strcat(res, previous_hash), "$");
    strcat(strcat(res, nonce), "$");

    //On ajoute à la chaine résultat les représentations des déclarations
    votes = block->votes;
    char* tempProtected;
    while (votes){
        tempProtected = protected_to_str(votes->data);
        strcat(res, strcat(tempProtected, "$"));
        free(tempProtected);
        votes = votes->next;
    }
    return res;
}

void delete_block(Block *b){
	//Supprime la mémoire occupée par un block.
	//Attention, cette fonction ne supprime pas la mémoire occupée par la clé auteur ni la mémoire occupée par les protected de la liste votes
    if (b->hash) free(b->hash);

    if (b->previous_hash != NULL) {
		free(b->previous_hash);
	}
    CellProtected* temp = b->votes;
    int i = 0;
    while(b->votes != NULL){
        i++;
        b->votes = b->votes->next;
        free(temp);
        temp = b->votes;
    }
    free(b);
}

unsigned char* hash_str(unsigned char* str){
	//crée une chaine de caractère en hachant une chaine de caractère avec la fonction SHA256
    char* result = (char*) malloc(sizeof(unsigned char) * 126);
    if (!result){
        printf("Erreur d'allocation (create_hashcell)\n");
        return NULL;
    }

    unsigned char* hashed = SHA256((unsigned char*) str, strlen((char*) str), 0);

    sprintf(result, "%02x", hashed[0]);

    char buffer[8];
    for (int i = 1; i < SHA256_DIGEST_LENGTH; i++){
            sprintf(buffer, "%02x", hashed[i]);
            strcat(result, buffer);
    }

    return (unsigned char*) result;
}

void compute_proof_of_work(Block* b, int d){
	//Calcule la PoW d'un bloc, elle boucle tant que la valeur hachée du bloc ne commence pas par d zéros
    if (!b) return;
    char* b_str = block_to_str(b);
    unsigned char* b_hashed = hash_str((unsigned char*)b_str);
	if (b->hash) free(b->hash);
    b->hash = b_hashed;
    free(b_str);

    if (has_n_zeros(b->hash, d)){
        return;
    } else{
        while (!has_n_zeros(b->hash, d)){
            unsigned char* new_hash = hash_str(b->hash);
            free(b->hash);
            b->hash = new_hash;
            b->nonce += 1;
        }
    }
}

int has_n_zeros(unsigned char* str, int n) {
	//Détermine si une chaine de caractère commence par n zéros
        for (int i = 0; i < n; i++) {
                if (str[i] != '0') return 0;
        }
        return 1;
}

int verify_block(Block* b, int d){
	//Vérifie la validité d'un bloc
    return has_n_zeros(b->hash, d);
}
