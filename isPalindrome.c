#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>

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

// Primero se valida que el archivo exista, después que no esté vacío en caso de ser archivo input
bool validFile(FILE *file, char modo, char *argopt) {
    if (file == NULL) {
        printf("El archivo %s no existe, por favor ingrese un archivo existente \n", argopt);
        return false;
    }

    if (empty(file) && modo != 'w') {
        printf("El archivo %s está vacío, por favor ingrese un archivo no vacío \n", argopt);
        return false;
    }

    printf("Se recibió el archivo %s \n", argopt);
    return true;
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
    printf("Las palabras palíndromas detectadas son: \n");
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
        cargarEnVectorPalabras(bufferLinea, palabras);  // carga en la matriz las palabras
        buscarPalindromos(palabras, outputFile);
        fgets(bufferLinea, MAXLINEA, inputFile);
    } 
    fclose(inputFile);

    printf("Se procesó el archivo de entrada \n");

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
        printf("Debe ingresar algún argumento, para mas información ingrese -h \n");
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
                if (!validFile(inputFile, 'r', optarg)) {
                    return 0;
                }
                break;
            case 'o':
                outputFile = fopen(optarg, "w");
                if (!validFile(outputFile, 'w', optarg)) {
                    return 0;
                }
                break;
            default:
                printf("Opción inválida. Para ver más información ingrese -h. \n");
        }
    }

    if (inputFile == NULL) {
        printf("Ingrese el stream a procesar (máximo 300 caracteres): \n");
        scanf("%*c%[^\n]",inputByStd);
        inputFile = fopen(inputFileAux, "w+");
        fputs(inputByStd, inputFile);
        takeStreamFromStdIn = true;
    }

    if (outputFile == NULL) {
        printf("Se mostrará el resultado en pantalla. \n");
        outputFile = fopen(outputFileAux, "w+");
        showResultsInStdOut = true;
    }

    processInput(inputFile, outputFile, showResultsInStdOut);

    // Borramos los archivos auxiliares utilizados
    if (takeStreamFromStdIn) remove(inputFileAux);
    if (showResultsInStdOut) remove(outputFileAux);

    return 0;
}