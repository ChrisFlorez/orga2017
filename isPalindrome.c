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

#define MAXLINEA 260
#define MAXCHARS 300


bool isPalindrome(char *palabra) {
    int posInicial, posFinal;
    posFinal = strlen(palabra) - 1;
    for (posInicial = 0; posInicial < strlen(palabra) / 2; posInicial++, posFinal--) {
        if ((toupper(*(palabra + posInicial))) != (toupper(*(palabra + posFinal)))) {
            return false;
        }
    }
    return true;
}

int seekPalindromes(char **palabras, FILE *archivo, int cantidadPalabras) {
    int contadorPalabra = 0;
    while (contadorPalabra<cantidadPalabras) {
        if (isPalindrome(palabras[contadorPalabra])) {
            if(fputs(palabras[contadorPalabra], archivo)==EOF){
            	fprintf(stderr, "Error fputs: %s\n", strerror( errno ));
            	return ERROR;
            }
            if(fputs("\n", archivo)==EOF){
            	fprintf(stderr, "Error fputs: %s\n", strerror( errno ));
            	return ERROR;
            }
        }
        free(palabras[contadorPalabra]);
        contadorPalabra++;
    }
    return SALIDA_EXITOSA;
}

bool validCharacter(char character) {
    int asciiNumber = (int) character;
    if ((asciiNumber <= 57) && (asciiNumber >= 48)) {
        return true;
    }
    if ((asciiNumber <= 90) && (asciiNumber >= 65)) {
        return true;
    }
    if ((asciiNumber <= 122) && (asciiNumber >= 97)) {
        return true;
    }
    if (asciiNumber == 45) {
        return true;
    }
    if (asciiNumber == 95) {
        return true;
    }
    return false;
}
char *agregarCaracterAVector(char caracter, char *vector, int contador){
	char *cadena = NULL;
	if(contador == 1){
		cadena = malloc(contador*sizeof(char));
		cadena[0] = caracter;

	}else{
		cadena = realloc(vector, contador * sizeof(char));
		cadena[contador-1]=caracter;
	}
	return cadena;
}
char **agregarPalabraAVector(char *palabra,char **palabras,int contDePalabrasGuardadas){
	char **auxiPalabras=NULL;
	if(contDePalabrasGuardadas == 1){
		auxiPalabras = malloc(contDePalabrasGuardadas*sizeof(char*));
		auxiPalabras[0] = palabra;
	}else{
		auxiPalabras = realloc(palabras, contDePalabrasGuardadas * sizeof(char*));
		auxiPalabras[contDePalabrasGuardadas-1] = palabra;
	}
	return auxiPalabras;
}
char** parseLine(char *linea, int *cantidadPalabras){
	char **palabras= NULL;
	char *palabra = NULL;
	bool salir = false;
	int contador = 0;
	int contDePalabrasGuardadas = 0;
	int contDeCaracteresGuardados = 0;
	while (salir == false) {
		if (validCharacter(linea[contador])) {
			contDeCaracteresGuardados++;
			palabra = agregarCaracterAVector(linea[contador], palabra,contDeCaracteresGuardados);
		}else if (contDeCaracteresGuardados != 0) {
			contDeCaracteresGuardados++;
			contDePalabrasGuardadas++;
			palabra = agregarCaracterAVector('\0', palabra,contDeCaracteresGuardados);
			palabras = agregarPalabraAVector(palabra,palabras,contDePalabrasGuardadas);
			contDeCaracteresGuardados=0;
		}
		if ((linea[contador] == '\n') || (linea[contador] == '\0')) {
			salir = true;
		}
		contador++;
	}
	*cantidadPalabras = contDePalabrasGuardadas;
	return palabras;
}
int processInput(FILE *inputFile, FILE *outputFile) {
    char bufferLinea[MAXLINEA];
    char **palabras = NULL;
    int cantidadPalabras=0;
    // para reposicionar el puntero del archivo a la primera linea
    // lectura anticipada del archivo para q no de mas lecturas
    rewind(inputFile);
    if ((fgets(bufferLinea, MAXLINEA, inputFile) == NULL) && (!feof(inputFile))) {
    	fprintf(stderr, "Error fgets: %s\n", strerror( errno ));
    	return ERROR;
    }
    while (!feof(inputFile)) {
        palabras = parseLine(bufferLinea, &cantidadPalabras);  // carga en la matriz las palabras
        if(seekPalindromes(palabras, outputFile,cantidadPalabras) == EOF){
        	return ERROR;
        }
        if((fgets(bufferLinea, MAXLINEA, inputFile) == NULL) && (!feof(inputFile))){
			fprintf(stderr, "Error fgets: %s\n", strerror(errno));
			return ERROR;
		}
        free(palabras);
    } 
    if (fclose(inputFile)==EOF) {
    	fprintf(stderr, "Error fclose: %s\n", strerror(errno));
    	return ERROR;
    }

    if (outputFile != stdout) {
    	if (fclose(outputFile) == EOF) {
    		fprintf(stderr, "Error fclose: %s\n", strerror(errno));
    		return ERROR;
    	}
    }
}

int main(int argc, char *argv[]) {
    int option = 0;
    const char *short_opt = "i:o:hV";
    struct option long_opt[] = {
            {"version", no_argument,       NULL, 'V'},
            {"help",    no_argument,       NULL, 'h'},
            {"input",   required_argument, NULL, 'i'},
            {"output",  required_argument, NULL, 'o'},
            {NULL, 0,                      NULL, 0}
    };
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    bool takeStreamFromStdIn = false;
    char inputByStd[MAXCHARS];
    char *inputFileAux = "inputFileAux.txt";

    if (argc == 1) {
        return 0;
    }

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
                printf("	-V, --version  Print version and quit. \n");
                printf("	-h, --help     Print this information. \n");
                printf("	-o, --output   Location of the output file. \n");
                printf("	-i, --input    Location of the input file. \n");
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
            default:
                abort();
        }
    }

    if (inputFile == NULL) {
        fgets(inputByStd,MAXCHARS,stdin);
        inputFile = fopen(inputFileAux, "w+");
        fputs(inputByStd, inputFile);
        fputs("\n", inputFile);
        takeStreamFromStdIn = true;
    }

    if (outputFile == NULL) {
        outputFile = stdout;
    }

    if (processInput(inputFile, outputFile) == ERROR) {
    	return ERROR;
    }

    // Borramos los archivos auxiliares utilizados
    if (takeStreamFromStdIn) remove(inputFileAux);

    return SALIDA_EXITOSA;
}
