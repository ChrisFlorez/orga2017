#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define ERROR -1
#define SALIDA_EXITOSA 0

int archiDescriptorInt;
int archiDescriptorOut;

extern int palindrome(int ifd, size_t ibytes, int ofd, size_t obytes);

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

    if (obytes == NULL) obytes = "1";
    archiDescriptorInt = fileno(inputFile);
    archiDescriptorOut = fileno(outputFile);
    palindrome(archiDescriptorInt, (size_t)atoi(ibytes), archiDescriptorOut, (size_t)atoi(obytes));

    if(inputFile != stdin){
    	if (fclose(outputFile) == EOF) {
			fprintf(stderr, "Error fclose: %s\n", strerror(errno));
			return ERROR;
		}
	}
    if(outputFile != stdout) {
        if (fclose(outputFile) == EOF) {
    		fprintf(stderr, "Error fclose: %s\n", strerror(errno));
    		return ERROR;
    	}
    }
    return SALIDA_EXITOSA;
}
