# Compte rendu projet structures de données LU2IN009

>Natacha Rivière et Romain Demont

Ce projet s'inscrit dans le cadre de l'UE *structures de données*, nous avons réalisé une blockchain pour simuler une élection par scrutin uninominal majoritaire à deux tours.  Ce projet est réalisé dans le langage C. Il s'appuie sur le cryptage des déclarations de vote à l'aide du protocole RSA.

### Table des matières


[TOC]

### Structures utilisées

````c
 typedef struct key{
  long n;
  long val;
 } Key;
````
La structure `Key` permet de stocker les deux entiers premiers entre eux utilisés pour crypter ou décrypter une déclaration de vote à l'aide du protocole RSA.

````c
 typedef struct signature{
  int size;
  long* tab;
 } Signature ;
````

La structure `Signature` contient un tableau d'entier (long) contenant la version cryptée d'un message, ainsi que la taille de ce tableau.

````c
 typedef struct protected{
  Key* pKey;
  char* mess;
  Signature* sig;
 }Protected;
````

La structure `Protected` contient la clé publique du votant, une chaine de caractère contenant la clé publique du candidat choisi ainsi que la version cryptée à l'aide de la clé privée de l'électeur de cette chaine de caractère. Cette structure permet de contenir toutes les informations nécéssaires à une déclaration de vote valide.

```c
typedef struct cellKey{
    Key* data;
    struct cellKey* next;
} CellKey;
```

La structure `CellKey` permet de construire une cellule de liste chainée de `Key`.

```c
typedef struct cellProtected{
  Protected* data;
  struct cellProtected* next;
}CellProtected;
```

La structure `CellProtected` permet de construire une cellule de liste chainée de `Protected`.

```c
typedef struct hashcell{
  Key* key;
  int val;
} HashCell;
```

La structure `HashCell` permet de construire une cellule de table de hachage stockant la clé publique d'un électeur et un entier valant 1 s'il a voté ou 0 sinon ou bien la clé publique d'un candidat ainsi que le nombre de voix comptabilisées en sa faveur.

```c
typedef struct hashtable{
  HashCell** tab;
  int size;
} HashTable;
```

La structure `HashTable` contient un tableau de `HashCell`, ainsi que sa taille. Elle permet de créer une table de hachage.

```c
typedef struct block{
  Key* author;
  CellProtected* votes;
  unsigned char* hash;
  unsigned char* previous_hash;
  int nonce;
}Block;
```

La structure `Block` permet de stocker les informations nécéssaires à la blockchain des éléctions. Elle contient une liste de déclarations de vote, la clé d'un assesseur, une preuve de travail permettant de vérifier la validité d'un bloc, ainsi que la valeur hachée su bloc qui le précède.

```c
typedef struct block_tree_cell{
  Block* block;
  struct block_tree_cell* father;
  struct block_tree_cell* firstChild;
  struct block_tree_cell* nextBro;
  int height;
}CellTree;
```

La structure `CellTree` permet de construire un arbre représentant une blockchain. Elle contient un bloc, un pointeur sur la `CellTree` la précédant dans la blockchain, un pointeur sur celle qui la suit ainsi que un pointeur sur les autres `CellTree` de même père. Elle a ausi une hauteur qui permet de déterminer quelle est la blockchain valide.

### Description générale

Le projet est séparé en 9 fichier contenant le code des différentes fonction utilisées. Les fonctions sont rangées par exercice. Chaque fichier c est accompagné d'un fichier h contenant le prototype de ces fonctions. Les fichiers h des exercices s'incluent en cascade, `e1.h` est inclu dans `e2.h` qui est inclu dans `e3.h`, etcaetera. Les fichiers contenants les fonctions sont situés dans le dossier Fonctions. 

 - `e1.c`


Le fichier `e1.c` contient les fonctions nécéssaires afin de générer des nombres premiers aléatoires grâce à la fonction `random_prime_number`. 

Il contient les fonctions suivantes :

```c
int is_prime_naive(long n);
long modpow_naive(long a, long m, long n);
long modpow(long a, long m, long n);
int witness(long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_miller(long p, int k);
long random_prime_number(int low_size, int up_size, int k);
```

- `e2.c`

Le fichier `e2.c` contient les fonctions nécéssaires afin de générer des couples de nombres pouvant être utilisés comme clés ainsi que les fonctions permettant de crypter et décrypted un message avec ces valeurs.

Il contient les fonctions suivantes :

```c
long extended_gcd(long s, long t, long* u, long* v);
void generate_key_values(long p, long q, long* n, long* s, long* u);
long* encrypt(char* chaine, long s, long n);
char* decrypt(long* crypted, int size, long u, long n);
void print_long_vector(long* result, int size);
```

- `e3.c`

Le fichier `e3.c` contient les fonctions nécéssaires pour manipuler les structures `Key`, `Signature` et `Protected` ainsi que la fonction permettant de vérifier la validité d'une déclaration de vote `verify`.

Il contient les fonctions suivantes :

```c
void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key); 
Key* str_to_key(char* str); 

Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str ( Signature* sgn );
Signature* str_to_signature ( char* str );

Protected* init_protected(Key* pKey, char* mess, Signature* sig);
int verify(Protected* pr);
char* protected_to_str( Protected* pr );
Protected* str_to_protected(char* str);

void free_signature(Signature* sig);
void free_protected(Protected* pr);
```

- `e4.c`

Le fichier `e4.c` contient la fonction `generate_random_data` qui utilise les fonctions des fichiers précédents pour créer trois fichiers. Le premier, `keys.txt` contient une liste de couples de clés, privées et publiques, représentant tous les électeurs. Le deuxième, `candidates.txt` contient une liste de clés publiques des candidats à l'élection, choisi aléatoirement dans le premier fichier. Le dernier, `declarations.txt` contient les déclarations de vote de tous les électeurs.

Il contient les fonctions suivantes :

```c
void generate_random_data(int nv, int nc);
int is_in(int r, int * tab, int size);
```

- `e5.c`

Le fichier `e5.c` contient les fonctions nécéssaires afin de créer des listes chaînées de clés avec la structure `CellKey` ou des listes chaînées de Protected avec la structure `CellProtected`. Ces fonctions lisent dans les fichiers générés par `generate_random_data`.

Il contient les fonctions suivantes :

```c
CellKey* create_cell_key(Key* key);
void add_key_front(CellKey** list, Key* key);
CellKey* read_public_keys(char* file);
void print_list_keys(CellKey* list);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* list);

CellProtected* create_cell_protected(Protected* pr);
void add_pr_front(CellProtected** list, Protected* pr);
CellProtected* read_protected();
void print_list_protected(CellProtected* list);
void delete_cell_pr(CellProtected* c);
void delete_list_pr(CellProtected* list);
```

- `e6.c`

Le fichier `e6.c` contient les fonctions permettant de vérifier la validité d'une liste chaînée de `Protected` et de retirer les déclarations de vote non valides, de créer des tables de hachage permettant de compter rapidement les voix et de vérifier que chaque personne ne vote pas plus d'une fois. Enfin, la fonction `compute_winner` utilise la table de hachage des candidats pour déterminer le gagnant de l'élection.

Il contient les fonctions suivantes :

```c
CellProtected* verify_list(CellProtected* liste);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
int is_key_identical(Key* k1, Key* k2);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
Key* count_winner(HashTable* t, int size);
```

- `e7.c`

Le fichier `e7.C` contient les fonctions permettant de générer des blocs valides, de les lire et de les écrire dans des fichiers.

Il contient les fonctions suivantes :

```c
void block_to_file(char* fileName, Block* b);
Block* file_to_block(char* fileName);
char* block_to_str(Block* block);
void delete_block(Block* b);
unsigned char* hash_str(unsigned char* str);
void compute_proof_of_work(Block* b, int d);
int verify_block(Block* b, int d);
int has_n_zeros(unsigned char* str, int n);
```

- `e8.c`

Le fichier `e8.c` contient les fonctions permettant de créer un arbre, de le mettre à jour et de le supprimer.

Il contient les fonctions suivantes :

```c
CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
//deux fonction annexes
void add_child_recursif(CellTree* child, CellTree* new);
void update_height_recursif(CellTree* cell);

void print_tree(CellTree* tree);
void delete_node(CellTree* node);
void delete_tree(CellTree* tree);

CellTree* highest_child(CellTree* tree);
CellTree* last_node(CellTree* tree);
CellProtected* fusion_lists(CellProtected* a, CellProtected* b);
CellProtected* fusion_lists_votes(CellTree* tree);
```

- `e9.c`

Le fichier `e9.c` contient les fonctions permettant de gérer une blockchain pour des élections. Notamment pour soumettre des votes, les ajouter à la blockchain et calculer le gagnant d'une élection. Il contient également la fonction permettant de reconstituer l'arbre de la blockchain à partir des fichiers des blocks.

Il contient les fonctions suivantes :

```c
void submit_vote(Protected* p);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree();
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
```

### Nos algorithmes

Certaines fonctions de ce projet utilisent des algorithmes que nous avons mis au point, cette rubrique donne une description générale de leur fonctionnement.

- `random_prime_number`

  Cette fonction génère un nombre premier dans un intervalle donné.

  Pour ce faire, elle génère un nombre aléatoire dans cet intervalle et renvoie le premier nombre premier supérieur ou égal à ce nombre qu'elle trouve. Si aucun nombre premier plus grand que ce nombre n'existe, elle renvoie le premier nombre premier inférieur à ce nombre. Elle renvoie `-1` si aucun nombre premier n'existe dans l'intervalle.

- `generate_random_data`

  Cette fonction écrit trois fichiers contenant les clés des électeurs, les clés publiques des candidats puis les déclarations de vote des électeurs.

  Elle commence par écrire le fichier `keys.txt` en générant des `Key` et en les écrivant dans le fichier. Ensuite, elle génère une liste de candidats tous différents. Elle produit des nombres aléatoires, qu'elle stocke dans un tableau et récupère la clé publique écrite à cette ligne dans le fichier. Si le nombre était déjà dans le tableau de nombre produits, elle en reproduit un autre. Si le nombre de candidats demandé est supérieur au nombre de votants, le nombre de candidats sera le nombre de votants.

  La fonction simule ensuite le vote, elle lit ligne par ligne dans le fichier `keys.txt` pour récupérer les clés de l'électeur. Elle génère un nombre aléatoire entre 1 et le nombre de candidats et va lire dans `candidates.txt` la clé publique de ce candidat. Elle la crypte et crée un `Protected` qu'elle transforme en chaine de caractère qu'elle écrit dans le fichier `declarations.txt`.  Si la déclaration stockée dans le `Protected` n'est pas valide, elle n'est pas écrite dans le fichier `declarations.txt`.

- `verify_list`
  
  Cette fonction vérifie la validité des déclarations de vote d'une liste chaînée `CellProtected` et retire les déclarations invalides.
  
  Elle fait une boucle sur tous les éléments de la liste à partir du deuxième et les retire s'ils sont invalides. Elle gère ensuite le cas du premier élément. Ainsi, s'il est invalide, on évite de devoir gérer le cas où le premier est toujours invalide puisque c'est un cas à part comme il n'y a pas de précédent à relier à la liste.
  
- `find_position`

  Cette fonction cherche dans une table de hachage une clé et renvoie la première case vide où la ranger si elle n'est pas présente dans la table.

  La fonction itère sur la table de hachage en partant de la position renvoyée par la fonction de hachage. Tant qu'elle ne tombe pas sur la clé demandée ou sur une case vide, elle continue. Si elle trouve la clé ou une case vide, elle la renvoie. Si elle itère toit le tableau et revient à la position initiale, elle renvoie `-1`.

- `compute_winner`

  Cette fonction utilise une liste chaînée `CellProtected` pour compter les voix et déterminer le gagnant d'une élection. La liste cchaînée utilisée est supposée valide.

  Elle vérifie d'abord que le votant est inscrit dans les listes électorales, c'est à dire qu'il est présent dans la table de hachage des électeurs grâce à la fonction `find_position`. Si c'est le cas et qu'elle n'a pas encore voté, elle note que cette personne a voté (elle passe la valeur val à 1) et va compter le vote.

  Elle vérifie donc que la personne pour qui cet électeur vote est bien candidat (c'est à dire qu'elle est dans la table de hachage des candidats) et augmente le compte de voix de ce candidat.

  Elle utilise ensuite la fonction auxiliaire `count_winner` pour déterminer quel candidat a le plus de voix.  Cette fonction parcoure la table de hachage des candidats et renvoie la clé publique du candidat ayant le plus de voix. En cas d'égalité, elle renvoie le premier du tableau ayant le compte maximal.

### Réponses aux questions

##### Exercice 1

1. La complexité de la fonction `is_prime_naive` est O(p)
2. Le plus grand entier testé avec cette fonction en moins de 2 milisecondes est 113489. (C'est le premier entier qui prend plus de 2 milisecondes à être testé.)
3. La complexité de la fonction `modpow_naive` est en O(m). La fonction `modpow_naive` calculant $\ a^m \% n $.

###### Comparaison des deux méthodes d'exponentiation modulaire

![Comparaisons Modpow](/home/natacha/Dropbox/SU/S4/LU2IN006/Projet/comparaisonModpows.png)

On constate que le temps que prend la fonction `modpow_naive` augmente linéairement avec la valeur de l'exposant tandis que le temps que prend la fonction `modpow` est toujours le même à epsilon près et est extrêmement court. La fonction `modpow` est donc beaucoup plus optimisée, elle est donc préférée à sa version naive.

7. La probabilité d'erreur de cet algorithme est ${1 \over 4}^k$, donc si k est très grand, la probabilité est très faible.



##### Exercice 7

8. Le temps de calcul dépasse une seconde lorsque l'on demande plus de 4 zéros à la fonction. 

![Temps de compute_proof_of_work](/home/natacha/Dropbox/SU/S4/LU2IN006/Projet/Temps_compute_proof_of_work.png)


##### Exercice 8

8. La fonction fusion_lists est en complexité O(n) avec n la taille de la liste chainée finale. Il faudrait faire une liste doublement chainée pour avoir une complexité en O(1).

##### Exercice 9

7. L'utilisation d'une blockchain dans le cadre d'un processus de vote semble sécurisée mais le vote n'est pas anonyme. Faire confiiance à la plus longue chaine permet d'éviter les fraudes tant qu'un ordinateur beaucoup plus puissant n'est pas utilisé pour frauder. Car il pourrait avoir suffisamment de puissance de calcul pour aller aussi vite que la blockchain. Mais c'est très improbable car il faudrait une puissance de calcul démentielle.

### Description des jeux d'essais

Les jeux d'essais sont répartis dans 9 fichiers `mainsX.c` correspondant chacun à un exercice du projet situés dans le dossier Tests. Lors de la compilation, les éxécutables se trouvent dans le dossier Exécutables.

- `main1.c`

Le fichier `main1.c` est constitué de deux parties, la première itère sur les entier avec la fonction `is_prime_naive` tant que le temps de calcul est inférieur à deux milisecondes. La deuxième écrit dans des fichiers le temps de calcul des deux fontions `modpow_naive` et `modpow` afin de pouvoir générer la courbe de la question 1.3.

Enfin, elle affiche un nombre premier au hasard grâce à la focntion `random_prime_number`.

- `main2.c` 

Le fichier `main2.c` génère un couple de clés, publique et privée et crypte le message _Hello World_ avec ces clés, le décrypte et l'affiche.

-  `main3.c` 

Le fichier `main3.c` génère des clés puis une signature et un Protected , les affiche et les lis. Il vérifie également que le Protected généré est valide.

- `main4.c` 

Le fichier `main4.c` génère les fichiers _keys.txt_, _candidates.txt_ et _declarations.txt_ utilisés pour les votes des électeurs et les affiche.

- `main5.c`

Le fichier `main5.c` lis dans les fichiers générés par `generate_random_data` et crée des listes chaînées de clés (`CellKey*`) et une liste chaînée de Protected (`CellProtected*`) à partir de cette lecture. Il les affiche ensuite.

- `main6.c`

Le fichier `main6.c` crée des listes chaînées de clés et de protected de la même manière que `main5.c` puis vérifie les déclarations de vote qu'il a lu, les supprime si elles ne sont pas valides. Il compte ensuite les votes et affiche le gagnant des élections en utilisant une table de hachage pour ranger et compter les déclarations de vote, il utilise pour celà la focntion `compute_winner`.

- `main7.c` 

Le fichier `main7.c` lis dans le fichier _block.txt_ le contenu d'un bloc, l'écrit dans un fichier, qu'il affiche. Ensuite il calcule la proof of work associée à ce bloc pour tous les entiers entre 0 et 7. Il écrit dans le fichier _benchmark_SHA256_ les temps nécéssaires à ces calculs.

- `main8.c`

Le fichier `main8.c` crée un arbre à partir de deux fichiers de bloc _block.txt_ et _block1.txt_ sachant que le deuxième est le fils du premier. Il utilise ensuite les fonctions `highest_child` et `last_node` sur l'arbe. Enfin, il fusionne les votes de l'abre et les affiche.

- `main9.c`

Le fichier `main9.c` génère des fichiers à l'aide de `generate_random_data` et les lit pour constituer des listes chaînées. Ensuite, il soumet des blocs de dix votes en parcourant la liste des protected. Par défaut, l'assesseur est le premier citoyen de la liste des votants. A chaque soumission de dix votes, il ajoute les nouveaux votes à la blockchain, dans le dossier Blockchain. Il lit l'arbre créé à l'aide de `read_tree`, et continue à lire les nouveaux votes. Une fois la liste de protected entièrement parcourue, il récupère les votes de la chaine la plus longue de la blockchain et détermine le gagnant de l'élection à l'aide de `compute_winner_BT`.
