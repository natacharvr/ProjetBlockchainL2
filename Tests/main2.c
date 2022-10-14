#include "../Fonctions/e2.h"

int main(){
	srand(time(NULL));

     //Generation de cle :
     long p = random_prime_number(3 ,7 , 5000);
     long q = random_prime_number(3 ,7 , 5000);
     while (p == q) {
         q = random_prime_number(3 ,7 , 5000);
     }
     long n, s, u;
     generate_key_values(p, q, &n, &s, &u);
	 //Pour avoir des cles positives :
     if (u < 0) {
         long t = (p - 1) * (q - 1);
         u = u + t; //on aura toujours s * u mod t = 1
     }
     printf ("cle publique = (%ld, %ld) \n", s, n);
     printf ("cle privee = (%ld, %ld) \n", u, n);

     char message[100] = "Hello World !";
     int len = strlen(message);

     //Chiffrement:
     long* crypted = encrypt(message, s, n);

     printf("Initial message: %s \n", message);
     printf("Encoded representation: \n");
     print_long_vector(crypted, len);

     //Dechiffrement
     char* decoded = decrypt(crypted, len, u, n);
     printf ("Decoded : %s \n", decoded);
     free(decoded);
     free(crypted);
}
