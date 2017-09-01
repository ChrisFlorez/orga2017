#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
void verificarPalabra(char palabra[30]) {
	
}
 
int main()
{
    char palabra[20];
    int posInicial, posFinal;
    int palindromo = 1;
    printf("Escribe una palabra: ");
    fflush(stdout);
    gets(palabra);
    posFinal = strlen(palabra)-1;
    for (posInicial = 0; posInicial < strlen(palabra)/2; posInicial++, posFinal--) {
        printf ("Comprobando %c==%c\n", *(palabra+posInicial), *(palabra+posFinal));
        if ((toupper(*(palabra + posInicial))) != (toupper(*(palabra + posFinal)))) {
            palindromo = 0;
            break;
        }
    }
    if (palindromo)
        printf("\nEs un palíndrimo.\n");
    else
        printf("\nNo es un palíndrimo.\n");
 
    return (0);
}
