#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern int strlcpy(char *destino, char *origen,int caracteresCopia);
extern void *mymalloc(size_t);
extern void myfree(void *);
extern int palindromo(char *cadena);
extern int mystrlen(char *cadena);


extern char* agregarCaracter(char* cadena,char caracterExtradido,int cantCaracteres);/*{
	char* cadenaAuxi = mymalloc(cantCaracteres);
	if(cadena != NULL){
		strlcpy ( cadenaAuxi , cadena , cantCaracteres);
	}
	cadenaAuxi[cantCaracteres-1] = caracterExtradido;
	return cadenaAuxi;
}*/
extern int validCharacter(char character);/* {
    int asciiNumber = (int) character;
    if ((asciiNumber <= 57) && (asciiNumber >= 48)) {
        return 1;
    }
    if ((asciiNumber <= 90) && (asciiNumber >= 65)) {
        return 1;
    }
    if ((asciiNumber <= 122) && (asciiNumber >= 97)) {
        return 1;
    }
    if (asciiNumber == 45) {
        return 1;
    }
    if (asciiNumber == 95) {
        return 1;
    }
    return 0;
}*/
extern int seFormoUnaPalabra(char *cadena,int cantidadCaracteres);/*{
	int posicionDelUltimoCaracter = cantidadCaracteres-1;
	int caracter =(int)cadena[posicionDelUltimoCaracter];
	if(validCharacter(caracter) == 0){
		if(cantidadCaracteres==1){
			return 2;
		}
		return 1;
	}
	return 0;
}*/
int palindromes(int archivoIn, int tamanioIn,int archivoOut,int tamanioOut){
	char *bufferEntrada = mymalloc(tamanioIn);
	char *bufferSalida = mymalloc(tamanioOut);
	int	contadorDeBufferSalida = 0;
	int cantCaracteres = 0;
	char *cadenaDeCaracteres = NULL;
	int salir = 1; //1 sigue, 0 sale
	while(salir == 1){
		int cantidadCaracteresLeidos = read(archivoIn,bufferEntrada,tamanioIn);
		if(cantidadCaracteresLeidos  != tamanioIn){
			salir = 0;
		}
		//aca proceso el buffer de entrada copiando byte en byte
		int contadorDeBufferLeidos = 0;
		while((contadorDeBufferLeidos < cantidadCaracteresLeidos) && (cantidadCaracteresLeidos>0)){
			cantCaracteres++;

			char caracterExtraido = bufferEntrada[contadorDeBufferLeidos];
			cadenaDeCaracteres = agregarCaracter(cadenaDeCaracteres,caracterExtraido,cantCaracteres);

			if(seFormoUnaPalabra(cadenaDeCaracteres,cantCaracteres) == 1){
				cadenaDeCaracteres[cantCaracteres-1] = '\0';
				if(palindromo(cadenaDeCaracteres) == 1){
					int contador = 0;
					while(contador<(cantCaracteres-1)){
						if(contadorDeBufferSalida==tamanioOut){
							write(archivoOut,bufferSalida,tamanioOut);
							contadorDeBufferSalida = 0;
						}else{
							bufferSalida[contadorDeBufferSalida] = cadenaDeCaracteres[contador];
							contadorDeBufferSalida++;
							contador++;
						}

					}
					bufferSalida[contadorDeBufferSalida] = '\n';
					contadorDeBufferSalida++;
				}
				myfree(cadenaDeCaracteres);
				cantCaracteres = 0;
				cadenaDeCaracteres = NULL;
			}else if(seFormoUnaPalabra(cadenaDeCaracteres,cantCaracteres) == 2){
				myfree(cadenaDeCaracteres);
				cantCaracteres = 0;
				cadenaDeCaracteres = NULL;
			}
			contadorDeBufferLeidos++;

		}
		//write(1,bufferEntrada,cantidadCaracteresLeidos);
	}
	write(archivoOut,bufferSalida,contadorDeBufferSalida);
	myfree(bufferEntrada);
	myfree(bufferSalida);
	return 0;
}
int main()
{
	/*char* vector = "0";
	printf("%d\n",palindromo(vector));
	char *cadena = "!";
	if(seFormoUnaPalabra(cadena,1) == 1){

		printf("se formo palabra\n");
		write(1,cadena,1);
	}else if(seFormoUnaPalabra(cadena,1) == 0){
		printf("no se formo palabra\n");
	}else{
		printf("buffer muy chico\n");
	}*/
	/*char *cadena = "ana\n";
	if(seFormoUnaPalabra(cadena,4)==1){
		write(1,cadena,4);
	}else if(seFormoUnaPalabra(cadena,4)==2){
		write(1,"invalido",8);
	}else{
		printf("no se formo una palabra\n");
	}*/
	//write(1,cadena,4);
	FILE *archivoEntrada = fopen("lInt.txt","r");
	FILE *archivoSalida = fopen("lout.txt","w");
	int tamanioIn = 4;
	int tamanioOut = 4;
	if(archivoEntrada == NULL || archivoSalida == NULL){
		printf("no exite archivo\n");
	}else{
		palindromes(fileno(archivoEntrada),tamanioIn,1,tamanioOut);
		printf("salio\n");
	}
	return 0;
}
