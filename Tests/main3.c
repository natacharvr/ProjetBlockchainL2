#include "../Fonctions/e3.h"

int main(){
	srand(time(NULL));

	//test de génération et de lecture/ écriture des clés
	Key* pKey = malloc(sizeof(Key));
	Key* sKey = malloc(sizeof(Key));

	init_pair_keys(pKey, sKey, 5, 7);

	char* pKey_str = key_to_str(pKey);
	printf("%s\n", pKey_str);
	Key* k = str_to_key("(22b1, a3)");
	printf("(%ld, %ld)\n", k->n, k->val);

	//test de génération et de lecture/ écriture des signatures
	Signature* sng = sign("HelloWorld", sKey);
	char* sng_str = signature_to_str(sng);
	printf("Signature : %s\n", sng_str);
	Signature* sng_read = str_to_signature(sng_str);
	char* sng_str2 = signature_to_str(sng_read);
	printf("Signature lue : %s\n", sng_str2);

	//test de génération et de lecture/ écriture des protected
	Protected* pr = init_protected(pKey, "HelloWorld", sng);
	char* pr_str = protected_to_str(pr);
	printf("Protected : %s\n", pr_str);

	if (verify(pr)){
		printf("Le protected est valide\n");
	} else {
		printf("Le protected est invalide\n");
	}
	Protected* pr2 = str_to_protected(pr_str);
	char* pr_str2 = protected_to_str(pr2);
	printf("Protected lu : %s\n", pr_str2);

	free(pr_str2);
	free(sng_str);
	free(sng_str2);
	free(pKey_str);
	free(pr_str);
	free_signature(sng_read);
	free(k);
	free(sKey);
	free_protected(pr);
	free_protected(pr2);
}
