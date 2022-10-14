#include "e3.h"

void init_key(Key* key, long val, long n){
	//Ecrit dans une clé déjà allouée les valeurs qui lui correspondent
    if (key == NULL){
        printf("key n'existe pas");
        return;
    }
    key->n = n;
    key->val = val;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
	//initialise une paire de clés, publique et privée aléatoirement
    long p = random_prime_number(low_size,up_size, 5000);
    long q = random_prime_number(low_size,up_size, 5000);
    while(p == q){
        q = random_prime_number(low_size, up_size, 5000);
    }
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u);
	//Pour avoir des cles positives :
    if(u < 0){
        long t = (p - 1) * (q - 1);
        u = u + t; //on aura toujours s*u mod t = 1
    }
    init_key(pKey, s, n);
    init_key(sKey, u, n);
}

char* key_to_str(Key* key){
	//Retourne une chaine de caractères représentant une clé
    char* res = malloc(sizeof(char) * TAILLE_KEY);
	if (!res){
		printf("Erreur d'allocation (key_to_str)\n");
		return NULL;
	}

    sprintf(res, "(%lx,%lx)", key->val, key->n);
    return res;
}

Key* str_to_key(char* str){
	//lis une clé dans une chaine de caractères
    Key* key = malloc(sizeof(Key));
	if (!key){
		printf("Erreur d'allocation (str_to_key)\n");
		return NULL;
	}

    long unsigned int a, b;
    sscanf(str, "(%lx,%lx)", &b, &a);
    init_key(key, b, a);
    return key;
}

Signature* init_signature(long* content, int size){
	//crée une signature contenant le content donné
    Signature* sig = malloc(sizeof(Signature));
	if (!sig){
		printf("Erreur d'allocation (init_signature)\n");
		return NULL;
	}

    sig->size = size;
    sig->tab = content;
    return sig;
}

Signature* sign(char* mess, Key* sKey){
	//génère une signature à partir d'une chaine de caractères en la cryptant avec la clé fournie
    long* tab = encrypt(mess, sKey->val, sKey->n);
    int i = strlen(mess);
    Signature* sig = init_signature(tab, i);
    return sig;
}

char* signature_to_str(Signature* sgn){
	//fourni
	//Retourne une chaine de caractère représentant la signature fournie
    char* result = malloc(10 * sgn->size * sizeof(char));
	if (!result){
		printf("Erreur d'allocation (signature_to_str)\n");
		return NULL;
	}

    result[0]= '#';
    int pos = 1;
    char buffer[256];
    for (int i = 0; i < sgn->size; i++){
        sprintf(buffer, "%lx", sgn->tab[i]);
        for (int j = 0; j < strlen(buffer); j++){
            result[pos] = buffer[j];
            pos = pos + 1;
        }
        result[pos] = '#';
        pos = pos + 1;
    }
    result[pos] = '\0';
    result = realloc(result,(pos + 1) * sizeof(char));
    return result;
}

Signature* str_to_signature(char* str){
	//fourni
	//lis une signature dans une chaine de caractères
    int len = strlen(str);
    long* content = (long*) malloc(sizeof(long) * len);
    if (content == NULL) {
        printf("erreur d'allocation");
        return NULL;
    }
    int num = 0;
    char buffer[256];
    int pos = 0;
    for(int i = 0; i < len; i++){
        if(str[i] != '#'){
            buffer[pos] = str[i];
            pos = pos + 1;
        } else{
            if(pos != 0){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc(content, num * sizeof(long));
    return init_signature(content, num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sig){
    Protected* proct = malloc(sizeof(Protected));
	if (!proct){
		printf("Erreur d'allocation (init_protected)\n");
		return NULL;
	}

    proct->pKey = pKey;
    proct->mess = strdup(mess);
    proct->sig = sig;
    return proct;
}

int verify(Protected* pr){
	//Vérifie qu'une déclaration est correcte en comparant la version cryptée fournie et la version cryptée du message donné
    char* mess2 = decrypt(pr->sig->tab, pr->sig->size, pr->pKey->val, pr->pKey->n);
    int a =(strcmp(pr->mess, mess2) == 0);
    free(mess2);
    return a;
}

char* protected_to_str(Protected* pr){
	//Retourne une chaine de caractères représentant le protected fourni
    char* res = malloc(sizeof(char) * TAILLE_PROTECTED);
	if (!res){
		printf("Erreur d'allocation (protected_to_str)\n");
		return NULL;
	}

    char* str1 = key_to_str(pr->pKey);
    char* str2 = signature_to_str(pr->sig);
    sprintf(res, "%s %s %s", str1, pr->mess, str2);
    free(str1);
    free(str2);
    return res;
}

Protected* str_to_protected(char* str){
	//Lis un protected (une déclaration) dans une chaine de caractères
    Protected* res = malloc(sizeof(Protected));
	if (!res){
		printf("Erreur d'allocation (str_to_protected)\n");
		return NULL;
	}

    char a[1000], b[1000], c[1000];
    sscanf(str, "%s %s %s", a, b, c);
    res->pKey = str_to_key(a);
    res->mess = strdup(b);
    res->sig = str_to_signature(c);
    return res;
}

void free_signature(Signature* sig){
	//Libère la mémoire allouée dans une signature
    free(sig->tab);
    free(sig);
}

void free_protected(Protected* pr){
	//Libère la mémoire allouée dans un Protected
    free(pr->pKey);
    free_signature(pr->sig);
    free(pr->mess);
    free(pr);
}
