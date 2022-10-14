#include "e1.h"

//premier entier de plus de 2ms : 113489

int is_prime_naive(long n){
    /*renvoie 1 si n est premier, 0 sinon, en testant toutes les
    divisions par les entiers inférieurs à n*/
    //printf("n : %ld\n", n);
    for (long i = 2; i <= n; i++){
        if (n % i == 0) return 0;
    }
    return 1;
}

long modpow_naive(long a, long m, long n){
    /*Calcule (a^m)%n itérativement*/
    long res = a;
    for (int i=1; i<m; i++){
        res = res * a;
        res = res % n;
    }
    return res;
}
//La complexité est 0(m)

long modpow(long a, long m, long n){
    /*Renvoie (a^m) % n de façon récursive pour optimiser les calculs*/
    if (m == 0) return 1 % n;
    if (m == 1){
        return a % n;
    }
    else {
        if (m % 2 == 0){
            long b = modpow(a, m / 2, n);
            return (b * b) % n;
        }
        else {
            long b = modpow(a, floor(m / 2), n);
            return (a * b * b) % n;
        }
    }
}

int witness(long a, long b, long d, long p){
	//fourni
    long x = modpow(a ,d, p);
    if(x == 1) {
        return 0;
    }
    for (long i = 0; i < b; i ++) {
        if (x == p -1) {
            return 0;
        }
        x = modpow (x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up){
	//fourni
    /*Renvoie un long aléatoire compris entre low et up*/
    return rand() % (up - low + 1) + low ;
}

int is_prime_miller(long p, int k){
	//fourni
    if(p == 2){
        return 1;
    }
    if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1)){ //tant que d n’est pas impair
        d = d / 2;
        b = b + 1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a ;
    int i ;
    for(i = 0; i < k ; i ++){
        a = rand_long (2, p - 1) ;
        if (witness(a, b, d, p)){
            return 0;
        }
    }
    return 1;
 }

/* La probabilité d'erreur de cet algorithme est (1/4)**k
donc si k est très grand, la probabilité est très faible*/

long random_prime_number(int low_size, int up_size, int k){
    /*génère un nombre aléatoire dans l'intervalle donné et renvoie le premier nombre premier supérieur à ce nombre.
    S'il n'y a pas de nombre premier supérieur à ce nombre dans l'intervalle, il renvoie le premier nombre premier inférieur
    Si pas de premier dans l'intervalle, renvoie -1*/
    long debut = pow(2, low_size);
    long fin = pow(2, up_size);
    long pivot = rand_long(debut, fin);
    long i = pivot;
    while (i <= fin){
        if (is_prime_miller(i, k)) return i;
        i += i & 1 ? 2 : 1; //si i est impair, on lui ajoute 2, sinon 1
    }
    while (i >= debut){
        if (is_prime_miller(i, k)) return i;
        i -= i & 1 ? 2 : 1; //si i est impair, on lui retire 2, sinon 1
    }
    return -1;
}
