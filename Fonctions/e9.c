#include "e9.h"

void submit_vote(Protected* p){
	//Ecrit une déclaration de vote à la fin du fichier Pending_votes.txt pour qu'elle soit traitée par un assesseur
    char* vote_a_ajouter = protected_to_str(p);
    FILE* f_pending_votes = fopen("Pending_votes.txt", "a");
    fprintf(f_pending_votes, "%s\n", vote_a_ajouter);
    fclose(f_pending_votes);
    free(vote_a_ajouter);
}

void create_block(CellTree* tree, Key* author, int d){
	//Ecrit un fichier Pending_bloc un bloc contenant les votes soumis par submit_vote (donc dans le fichier Pending_votes/txt)
	//Cette fonction prend en argument un arbre afin de créer un bloc qui a pour père le dernier de la liste la plus longue de l'arbre
	//Ainsi il crée un bloc valide qui s'insérera dans la Blockchain

    //On lit d'abbord le fichier et on le supprime
    FILE* f_pending_votes = fopen("Pending_votes.txt", "r");
    CellProtected* p_pending = read_protected(f_pending_votes);
    fclose(f_pending_votes);
    remove("Pending_votes.txt");

    //On crée le bloc valide
    Block* res = malloc(sizeof(Block));
	if (!res){
		printf("Erreur d'allocation (create_block)\n");
		return;
	}
    res->author = author;
    res->votes = p_pending;
    res->hash = NULL;
    CellTree* last = last_node(tree);
    if(last == NULL) res->previous_hash = NULL;
    else res->previous_hash = (unsigned char*)strdup((char*)last->block->hash);
    res->nonce = 0;

    compute_proof_of_work(res, d);

    //On écrit le bloc dans Pending_bloc
    block_to_file("Pending_bloc", res);
    delete_list_pr(res->votes);
	res->votes = NULL;
    delete_block(res);
}

void add_block(int d, char* name){
	//crée un fichier dans le dossier Blockchain contenant le bloc écrit dans Pending_bloc
    Block* b = file_to_block("Pending_bloc");
	//On vérifie que le bloc est valide avant de l'ajouter à la blockchain
    if (verify_block(b, d)){
        char path[100];
        sprintf(path, "../Blockchain/%s", name);
        block_to_file(path, b);
    }
    remove("Pending_bloc");
	delete_list_pr(b->votes);
	b->votes = NULL;
	free(b->author);
    delete_block(b);
}

CellTree* read_tree(){
	//Lit les fichiers situés dans le dossier Blockchain afin de créer un arbre contenant tous ses blocs

    //Première étape : créer un noeud pour chaque bloc du répertoire Blockchain
    DIR* rep = opendir("../Blockchain/");
    if (!rep) return NULL;

	//je commence par compter le nombre de fichiers pour allouer un tableau adapté.
    int count_files = 0;
    struct dirent* dir;

    while ((dir = readdir(rep))){
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
            count_files++;
        }
    }

    rewinddir(rep);
	CellTree* tab[count_files];

    for (int t = 0; t < count_files; t++) tab[t] = NULL;
    //je parcours alors les fichiers pour les ranger dans le tableau tab
    Block* b;
    char path[100];
    CellTree* cell;
    int i = 0;
    while ((dir = readdir(rep))){
        sprintf(path, "../Blockchain/");
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..")!= 0){
            strcat(path, dir->d_name);
            b = file_to_block(path);
            cell = create_node(b);
			//on trouve le père dans le tableau
            for (int j = 0; j < i; j++){
                if ((tab[j]->block->hash) && (cell->block->previous_hash) && (strcmp((char*)tab[j]->block->hash, (char*)cell->block->previous_hash)==0)){
                    cell->father = tab[j];
                }
            }
            tab[i] = cell;
            i++;
        }
    }
    closedir(rep);

    //Etape 2: on crée l'arbre, en ajoutant les fils à leurs pères.
    unsigned char* hash;
    for(int j = 0; j < count_files; j++){
        hash = tab[j]->block->hash;
        for (int k = 0; k < count_files; k++){
            if ((tab[k]->block->previous_hash) && (memcmp(tab[k]->block->previous_hash, hash, strlen((char*)hash)) == 0)){
                add_child(tab[j], tab[k]);
            }
        }
    }
    //Etape 3: on parcourt jusqu'à trouver la racine et on la retourne
    for (int j = 0; j < count_files; j++){
        if (tab[j]->father == NULL){
			return tab[j];
		}
    }
    printf("erreur : pas de racine\n");
    return NULL;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
	//Comptabilise les votes enregistrés dans la chaine la plus longue d'un arbre

    //utilise fusion_lists_votes
    CellProtected* list = fusion_lists_votes(tree);

    list = verify_list(list);
    Key* res = compute_winner(list, candidates, voters, sizeC, sizeV);
	delete_list_pr(list);
    return res;
}
