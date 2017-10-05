#include <stdio.h>
#include <stdlib.h>
extern int palindromo(char *palabra);
int main()
{
	char* palabra = "salas";
	printf("%s\n",palabra);
	if(palindromo(palabra) == 1){
		printf("palindroma\n");
	}else{
		printf("no palindromo\n");
	}
	printf("=======================\n");
	char* palabra1 = "hola";
	printf("%s\n",palabra1);
	if(palindromo(palabra1) == 1){
		printf("palindroma\n");
	}else{
		printf("no palindromo\n");
	}
	printf("=======================\n");
	char* palabra2 = "casa";
	printf("%s\n",palabra2);
	if(palindromo(palabra2) == 1){
		printf("palindroma\n");
	}else{
		printf("no palindromo\n");
	}
	printf("=======================\n");
	char* palabra3 = "amooma";
	printf("%s\n",palabra3);
	if(palindromo(palabra3) == 1){
		printf("palindroma\n");
	}else{
		printf("no palindromo\n");
	}
	return 0;
}
