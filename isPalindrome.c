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
bool isPalindrome( char *palabra) {
    int posInicial, posFinal;
    int palindromo = 1;
    posFinal = strlen(palabra) - 1;
    for (posInicial = 0; posInicial < strlen(palabra) / 2; posInicial++, posFinal--) {
        if ((toupper(*(palabra + posInicial))) != (toupper(*(palabra + posFinal)))) {
            palindromo = 0;
            break;
        }
    }
    if (palindromo) {
        return true;
    }
    
    return false;; //?? jiji, corregir
}
void buscarPalindromos(char palabras[][MAXLINEA],FILE *archivo) {
    int contadorPalabra = 0;
    while(palabras[contadorPalabra][0] != '$') {
        if (isPalindrome(palabras[contadorPalabra])) {
                fputs(palabras[contadorPalabra], archivo);
                fputs("\n", archivo);
        }
        contadorPalabra++;
    }
}

void guardarPalabraEnArchivo(char palabras[][MAXLINEA],FILE *archivo) {
	int contadorPalabra = 0;
    while(palabras[contadorPalabra][0]!='$'){
		fputs(palabras[contadorPalabra], archivo);
		fputs("\n", archivo);
        contadorPalabra++;
	}
}

void printPalindromos(FILE *archivo) {
    char bufferLinea[MAXLINEA];
    memset(&bufferLinea,0,MAXLINEA);
    rewind(archivo);
    fgets(bufferLinea, MAXLINEA,archivo);
        while (!feof(archivo)){
            printf("%s\n",bufferLinea);
            memset(&bufferLinea,0,MAXLINEA);
            fgets(bufferLinea, MAXLINEA,archivo);
        }

}

void imprimirPalabrasEnVector(char palabras[][260]) {
	int contadorPalabra = 0;
	while(palabras[contadorPalabra][0]!='$'){
		printf("%s\n",palabras[contadorPalabra]);
		contadorPalabra++;
	}
}
bool caracterValido(char caracter){
	int numeroAscii = (int)caracter;
	if((numeroAscii<=57)  && (numeroAscii>=48)){
		return true;
	}
	if((numeroAscii<=90)  && (numeroAscii>=65)){
		return true;
	}
	if((numeroAscii<=122)  && (numeroAscii>=97)){
		return true;
	}
	if(numeroAscii == 45){
		return true;
	}
	if(numeroAscii == 95){
		return true;
	}
	return false;
}

void cargarEnVectorPalabras(char *linea,char palabras[][260]){
	bool salir = false;
	int contador = 0;
	int contDePalabrasGuardadas = 0;
	int contDeCaracteresGuardados = 0;
	while(salir==false){
		if(caracterValido(linea[contador])){
			palabras[contDePalabrasGuardadas][contDeCaracteresGuardados]=linea[contador];
			contDeCaracteresGuardados++;
		}
		else if(contDeCaracteresGuardados != 0){
			palabras[contDePalabrasGuardadas][contDeCaracteresGuardados] = '\0';
			contDeCaracteresGuardados = 0;
			contDePalabrasGuardadas++;
		}
		if((linea[contador]=='\n') || (linea[contador]=='\0')){
			salir = true;
		}
		contador ++;
	}
	palabras[contDePalabrasGuardadas][0] = '$';
}

void procesarTexto( FILE *inputFile, FILE *outputFile, bool showResultsInStdOut){
    //se lee el archivo
    char bufferLinea[MAXLINEA];
    char palabras[MAXLINEA][MAXLINEA];
    rewind(inputFile);// para reposicionar el puntero del archivo a la primera linea
    //lectura anticipada del archivo para q no de mas lecturas
    do {
        fgets(bufferLinea, MAXLINEA,inputFile);
        cargarEnVectorPalabras(bufferLinea, palabras);//carga en la matriz las palabras
        buscarPalindromos(palabras,outputFile);
    } while (!feof(inputFile));

    fclose(inputFile);
    if(showResultsInStdOut) {
        printPalindromos(outputFile);//usamos rewind(outputFile) para llevar el indicador de posicion del archivo a la 1era linea.
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
        outputFile = fopen(auxOutputFile,"w+");
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
    procesarTexto(inputFile,outputFile, showResultsInStdOut);

    return 0;
}