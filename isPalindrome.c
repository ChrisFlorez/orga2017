#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MAXLINEA 260
#define MAXCHARS 300

// Verifica que el archivo no esté vacío
bool empty(FILE *file) {
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0) {
        return true;
    }

    fseek(file, savedOffset, SEEK_SET);
    return false;
}

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

void seekPalindromes(char palabras[][MAXLINEA], FILE *archivo) {
    int contadorPalabra = 0;
    while (palabras[contadorPalabra][0] != '$') {
        if (isPalindrome(palabras[contadorPalabra])) {
            fputs(palabras[contadorPalabra], archivo);
            fputs("\n", archivo);
        }
        contadorPalabra++;
    }
}

void printPalindromes(FILE *archivo) {
    char bufferLinea[MAXLINEA];
    memset(&bufferLinea, 0, MAXLINEA);
    rewind(archivo);
    fgets(bufferLinea, MAXLINEA, archivo);
    while (!feof(archivo)) {
        printf("%s", bufferLinea);
        memset(&bufferLinea, 0, MAXLINEA);
        fgets(bufferLinea, MAXLINEA, archivo);
    }
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

void parseLine(char *linea, char palabras[][MAXLINEA]) {
    bool salir = false;
    int contador = 0;
    int contDePalabrasGuardadas = 0;
    int contDeCaracteresGuardados = 0;
    while (salir == false) {
        if (validCharacter(linea[contador])) {
            palabras[contDePalabrasGuardadas][contDeCaracteresGuardados] = linea[contador];
            contDeCaracteresGuardados++;
        } else if (contDeCaracteresGuardados != 0) {
            palabras[contDePalabrasGuardadas][contDeCaracteresGuardados] = '\0';
            contDeCaracteresGuardados = 0;
            contDePalabrasGuardadas++;
        }
        if ((linea[contador] == '\n') || (linea[contador] == '\0')) {
            salir = true;
        }
        contador++;
    }
    palabras[contDePalabrasGuardadas][0] = '$';
}

void processInput(FILE *inputFile, FILE *outputFile, bool showResultsInStdOut) {
    char bufferLinea[MAXLINEA];
    char palabras[MAXLINEA][MAXLINEA];
    // para reposicionar el puntero del archivo a la primera linea
    // lectura anticipada del archivo para q no de mas lecturas
    rewind(inputFile);
    fgets(bufferLinea, MAXLINEA, inputFile);
    while (!feof(inputFile)) {
        parseLine(bufferLinea, palabras);  // carga en la matriz las palabras
        seekPalindromes(palabras, outputFile);
        fgets(bufferLinea, MAXLINEA, inputFile);
    } 
    fclose(inputFile);

    if (showResultsInStdOut) {
        printPalindromes(outputFile);//usamos rewind(outputFile) para llevar el indicador de posicion del archivo a la 1era linea.
    }
    fclose(outputFile);
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
    bool showResultsInStdOut = false;
    char inputByStd[MAXCHARS];
    char *inputFileAux = "inputFileAux.txt";
    char *outputFileAux = "outputFileAux.txt";

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
                break;
            case 'o':
	            if (access(optarg, W_OK) != -1) {
					outputFile = fopen(optarg, "w+");
				}
				break;
            default:
                abort();
        }
    }

    if (inputFile == NULL) {
        //printf("fopen failed to read, errno = %d\n", errno);
        fgets(inputByStd,MAXCHARS,stdin);
        inputFile = fopen(inputFileAux, "w+");
        fputs(inputByStd, inputFile);
        fputs("\n", inputFile);
        takeStreamFromStdIn = true;
    } else {
        if (empty(inputFile)) {
            printf("archivo vacio \n");
            return 0;
        }
    }

    if (outputFile == NULL) {
        outputFile = fopen(outputFileAux, "w+");
        showResultsInStdOut = true;
    }

    processInput(inputFile, outputFile, showResultsInStdOut);

    // Borramos los archivos auxiliares utilizados
    if (takeStreamFromStdIn) remove(inputFileAux);
    if (showResultsInStdOut) remove(outputFileAux);

    return 0;
}
