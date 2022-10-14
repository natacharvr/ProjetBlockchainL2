#include "e2.h"

long extended_gcd(long s, long t, long* u, long* v){
    /*fourni*/
    if (t == 0){
        *u = 1;
        *v = 0;
        return s;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd (t, s % t, &uPrim, &vPrim);
    *u = vPrim;
    *v = uPrim - (s / t) * vPrim;
    return gcd;
}

void generate_key_values(long p, long q, long* n, long* s, long* u){
	//génère les couples de clés (privées et publiques) d'un électeur
    *n = p * q;
    long v;
    long t = (p - 1) * (q - 1);

    do {
        *s = rand_long(0, t);
    }while (extended_gcd(*s, t, u, &v) != 1);

    while (*u < 2) *u = *u + t;
}

long* encrypt(char* str, long s, long n){
	//Utilise la clé publique pour encrypter un message
    long* res = malloc(sizeof(long) * strlen(str) + 2);
	if (!res){
		printf("Erreur d'allocation (encrypt)\n");
		return NULL;
	}

    for (int i = 0; str[i] != '\0'; i++) {
        res[i] = modpow((long) str[i], s, n);
    }
    return res;
}

char* decrypt(long* crypted, int size, long u, long n){
	//Utilise la clé privée pour décrypter un message
    char* message = (char*) malloc(sizeof(char) * (size + 1));
	if (!message){
		printf("Erreur d'allocation (decrypt)\n");
		return NULL;
	}
    int i;
    for (i = 0; i < size; i++){
        message[i] = (char) modpow(crypted[i], u, n);
    }

    message[i] = '\0';
    return message;
}

void print_long_vector(long* result, int size){
	/*fourni*/
    printf("Vector: [");
    for (int i = 0; i < size; i++){
        printf("%ld ", result[i]);
    }
    printf("]\n");
}
