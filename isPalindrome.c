#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXLINEA 260
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

void verificarPalabra(char palabra[30]) {

}

bool isPalindrome() {
    char palabra[20];
    int posInicial, posFinal;
    int palindromo = 1;
    printf("Escribe una palabra: ");
    fflush(stdout);
    gets(palabra);
    posFinal = strlen(palabra) - 1;
    for (posInicial = 0; posInicial < strlen(palabra) / 2; posInicial++, posFinal--) {
        printf("Comprobando %c==%c\n", *(palabra + posInicial), *(palabra + posFinal));
        if ((toupper(*(palabra + posInicial))) != (toupper(*(palabra + posFinal)))) {
            palindromo = 0;
            break;
        }
    }
    if (palindromo)
        printf("\nEs un palíndrimo.\n");
    else
        printf("\nNo es un palíndrimo.\n");

    return true; //?? jiji, corregir
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
    bool showResultsInStdOut = false;
    char inputByStd[300];
    char *auxFileName = "auxFile.txt";
    char *auxOutputFile = "auxOutput.txt";

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

    if (outputFile == NULL) {
        printf("Se mostrará el resultado en pantalla. \n");
        outputFile = fopen(auxOutputFile,"w+")
        showResultsInStdOut = true;
    }

    if (inputFile == NULL) {
        printf("Ingrese el stream deseado (máximo 300 caracteres): \n");
        gets(inputByStd);
        printf("Usted ingresó: %s \n", inputByStd);
        inputFile = fopen(auxFileName, "w+");
        fputs(inputByStd, inputFile);
        //fclose(inputFile);//conviene cerrarlo despues
        //printf("Borrando el archivo");
        //remove(auxFileName);
    }

    // aca es donde tenemos que leer del archivo que ingresaron o que generamos y escupir las palabras

    /*//se lee el archivo  
        char bufferLinea [MAXLINEA];
        while (!feof(inputFile)){
            
            //se lee una linea del archivo de entrada, se guarda en linea
            fgets(linea, MAXLINEA,inputFile);
            //se deberia usar el memset para limpiar el bufferLinea
            char **palabras = limpiarLinea(bufferLinea); //filtra la linea leida del archivo y la guarda en palabras
            buscarPalindromos(palabras,outputfile);
        }
        fclose(inputFile);
        
        if(showResultsInStdOut) {
            mostrarPalindromos(outputFile);//usamos rewind(outputFile) para llevar el indicador de posicion del archivo a la 1era linea.
        }
        fclose(outputFile);
    }*/



    return 0;
}
