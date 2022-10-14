#include "../Fonctions/e1.h"

int main(){
	srand(time(NULL));

	//Bloc permettant de tester le plus grand nombre que l'on peut tester avec is_prime_naive en moins de 2milisecondes
	clock_t t1, t2;
	double temps = 0;
	long n = 1;
	while (temps <= 0.002){
		printf("n : %ld\n", n);
		t1 = clock();
		is_prime_naive(n);
		//printf("%ld est premier : %d\n",n, is_prime_naive(n));
		t2 = clock();
		temps = (double)((t2-t1)) /CLOCKS_PER_SEC;
		printf("temps écoulé : %.3f\n", temps);
		n++;
	}
	printf("\nFinalement :\n n: %ld\ntemps : %.3f\n", n-1, temps);



	 //Code pour obtenir les courbes de comparaison de modpow_naive et modpow
	 double time;
	 FILE *f = fopen("../tempsModpow", "w");
	 FILE *f2 = fopen("../tempsNaive", "w");
	 for (int i = 0; i < 1000; i = i + 100){
		 printf("%d\n", i);
		 //modpow
		 t1 = clock();
		 modpow(34, i, 8);
		 t2 = clock();
		 time = (double)(t2-t1)/CLOCKS_PER_SEC;
		 fprintf(f, "%d %lf\n", i, time);
		 //naive
		 t1 = clock();
		 modpow_naive(34, i, 8);
		 t2 = clock();
		 time = (double)(t2-t1)/CLOCKS_PER_SEC;
		 fprintf(f2, "%d %lf\n", i, time);
	 }
	 fclose(f);
	 fclose(f2);

	 //test de la fonction de génération de nombre premier.
	 printf("Nombre premier :%ld\n",random_prime_number(15, 20, 10));
	return 1;
}
