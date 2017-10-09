#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#define ERROR -1
#define SALIDA_EXITOSA 0

extern int mystrlcpy(char *destino, char *origen,int caracteresCopia);
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
	//int	contadorDeBufferSalida = 0;
	int cantCaracteres = 0;
	char *cadenaDeCaracteres = NULL;
	int salir = 1; //1 sigue, 0 sale
	while(salir == 1){
		int cantidadCaracteresLeidos = read(archivoIn,bufferEntrada,tamanioIn);
		if(cantidadCaracteresLeidos  == 0){
			salir = 0;
			//printf("salir\n");
		}
		//write(archivoOut,bufferEntrada,cantidadCaracteresLeidos);
		//aca proceso el buffer de entrada copiando byte en byte
		int contadorDeBufferLeidos = 0;
		while((contadorDeBufferLeidos < cantidadCaracteresLeidos) && (cantidadCaracteresLeidos>0)){
			cantCaracteres++;

			char caracterExtraido = bufferEntrada[contadorDeBufferLeidos];
			cadenaDeCaracteres = agregarCaracter(cadenaDeCaracteres,caracterExtraido,cantCaracteres);
			//printf("%c\n",caracterExtraido);
			if(seFormoUnaPalabra(cadenaDeCaracteres,cantCaracteres) == 1){
				cadenaDeCaracteres[cantCaracteres-1] = '\0';
				if(palindromo(cadenaDeCaracteres) == 1){
					printf("%s\n",cadenaDeCaracteres);
					//int contador = 0;
					/*while(contador<(cantCaracteres-1)){
						if(contadorDeBufferSalida==tamanioOut){
							//write(archivoOut,bufferSalida,tamanioOut);
							contadorDeBufferSalida = 0;
						}else{
							bufferSalida[contadorDeBufferSalida] = cadenaDeCaracteres[contador];
							contadorDeBufferSalida++;
							contador++;
						}
					}
					bufferSalida[contadorDeBufferSalida] = '\n';
					contadorDeBufferSalida++;*/
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
	//write(archivoOut,bufferSalida,contadorDeBufferSalida);
	myfree(bufferEntrada);
	myfree(bufferSalida);
	return 0;
}

int main(int argc, char *argv[]) {

    int option = 0;
    char *ibytes = NULL, *obytes = NULL;
    const char *short_opt = "i:o:hVI:O:";
    struct option long_opt[] = {
            {"version",    no_argument,       NULL, 'V'},
            {"help",       no_argument,       NULL, 'h'},
            {"input",      required_argument, NULL, 'i'},
            {"output",     required_argument, NULL, 'o'},
            {"ibuf-bytes", required_argument, NULL, 'I'},
            {"obuf-bytes", required_argument, NULL, 'O'},
            {NULL, 0,                         NULL, 0}
    };
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    while ((option = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch (option) {
            case 'V':
                printf("TP #0 de la materia Organización de Computadoras \n");
                printf("Alumnos: \n");
                printf("	Flórez Del Carpio Christian\n	Montenegro Josefina \n	Quino Lopez Julian \n");
                return 0;
            case 'h':
                printf("Usage: \n");
                printf("	%s -h \n", argv[0]);
                printf("	%s -V \n", argv[0]);
                printf("	%s [options] \n", argv[0]);
                printf("Options: \n");
                printf("	-V, --version    Print version and quit. \n");
                printf("	-h, --help       Print this information. \n");
                printf("	-o, --output     Location of the output file. \n");
                printf("	-i, --input      Location of the input file. \n");
                printf("        -I, --ibuf-bytes Byte-count of the input buffer. \n");
                printf("        -O, --obuf-bytes Byte-count of the output buffer. \n");
                return 0;
            case 'i':
                inputFile = fopen(optarg, "r");
                if (inputFile == NULL) {
                    fprintf(stderr, "Error archivo entrada: %s\n", strerror(errno));
                }
                break;
            case 'o':
                // verifico si existe el archivo
                if (access(optarg, W_OK) != -1) {
                    outputFile = fopen(optarg, "w+");
                    if (outputFile == NULL) {
                        fprintf(stderr, "Error archivo salida: %s\n", strerror(errno));
                        return ERROR;
                    }
                }
                break;
            case 'I':
                ibytes = optarg;
                break;
            case 'O':
                obytes = optarg;
                break;
            default:
                // así está en el manual de getopt
                abort();
        }
    }

    if (inputFile == NULL) inputFile = stdin;

    if (outputFile == NULL) outputFile = stdout;

    if (ibytes == NULL) ibytes = "1";

    if (obytes == NULL) ibytes = "1";
    inputFile = fopen("lInt.txt", "r");
    //fileno(inputFile)
    palindromes(fileno(inputFile), 1, 1, 1);
    printf("Terminó el procesamiento. \n");

    return SALIDA_EXITOSA;

}
