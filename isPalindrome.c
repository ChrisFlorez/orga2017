#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//extern int lecturaEscritura(int archivo,char *buffer);
/*extern int llenarBufferEntrada(int archivoIn,int tamanioBufferIn,char* buffer){

	int cantidadCaracteresLeidos = read(archivoIn,buffer,tamanioBufferIn);
	//primero me fijo si hay error con a3
	if (cantidadCaracteresLeidos < 0){
		//retorno error tambien
	}
	return cantidadCaracteresLeidos;
}*/
/*extern int imprimirBuffer(int archivoOut,int tamanioBufferOut,char* buffer){
	return cantidadCaracteresEscritos
}*/
extern int palindrome(int archivoIn, int tamanioIn,int archivoOut);/*{
	char *bufferEntrada = malloc(tamanioIn * sizeof(char));
	//char *bufferSalida = malloc(tamanioOut * sizeof(char));
	int salir = 1; //1 sigue, 0 sale
	while(salir == 1){
		int cantidadCaracteresLeidos = llenarBufferEntrada(archivoIn,tamanioIn,bufferEntrada);
		if(cantidadCaracteresLeidos  != tamanioIn){
			salir = 0;
		}

		//procesoElBuffer(bufferSalida);
		write(1,bufferEntrada,cantidadCaracteresLeidos);
		printf("%d\n",cantidadCaracteresLeidos);
	}
	return 0;
}*/
int main()
{
	FILE *archivoEntrada = fopen("lInt.txt","r");
	FILE *archivoSalida = fopen("lout.txt","w");
	int tamanioIn = 4;
	if(archivoEntrada == NULL || archivoSalida == NULL){
		printf("no exite archivo\n");
	}else{
		palindrome(fileno(archivoEntrada),tamanioIn,fileno(archivoSalida));
		printf("salio\n");
	}
	return 0;
}
