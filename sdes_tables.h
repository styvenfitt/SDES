#ifndef __SDES_TABLES_H__
#define __SDES_TABLES_H__

#define BLOCK_SIZE 1

void sdesDescription(){

    printf("\t=======================================================================\n");
    printf("\t ============ Simplified Data Encription Standard (S-Des) ============");
    printf("\n\t=======================================================================\n\n");

    printf("\tBienvenido al panel de ayuda del programa, a continuacion se detalla\n\
    con brevedad una descripcion de los parametros que necesitamos pasar al\n\
    programa para que se ejecute asi como algun ejemplo.\n");

    printf("\t-e \tparametro para la opcion de encriptado, le precede el nombre del\n\
    \t\tfichero de entrada (texto, imagen, etc.) que se desea encriptar\n\
    \t\ty otro con la referencia al fichero de salida\n\
    \t-d \tparametro para la opcion de desencriptado, le precede el nombre\n\
    \t\tdel fichero de entrada con los datos a desencriptar y otro con la\n\
    \t\treferencia al fichero de salida\n\
    \t-k \tparametro que hace referencia a la clave que pasaremos en el fichero\n\
    \t\tcuyo nombre de referencia se debe poner a continuacion de este.\n\
    \t-x \tparametro para encriptar los datos que contiene un fichero\n\
    \t\ten hexadecimal cuyo nombre es el que precede a continuacion de este.\n\
    \t-h \tparametro que describe como se debe ejecutar el programa.\n");
    printf("\tEncriptado:\n\
    \t\t./sdes -e <in binary file> <out  binary file> -k <key file>\n\
    \t\t./sdes -x <in hexadecimal file> <out binary file> -k <key file>\n\
    \tDesencriptado:\n\
    \t\t./sdes -d <in binary file> <out binary file> -k <key file>\n");
}

unsigned char IP_TABLE[] = {
  2, 6, 3, 1, 4, 8, 5, 7 
};

unsigned char IP_INV_TABLE[]={
  4, 1, 3, 5, 7, 2, 8, 6 
};

unsigned char EXPANSION_TABLE[] = {
  4, 1, 2, 3, 2, 3, 4, 1 
};

#define S_GATES_COUNT 2
unsigned char S_GATES_TABLE[S_GATES_COUNT][4][4] = {
  // S0
  { {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
  },
  // S1
  { {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
  },
};

unsigned char P4_TABLE[] = {
  2, 4, 3, 1 
};

unsigned int KEY_SHIFT_TABLE[] = {
  1, 2
};

unsigned char P10_TABLE[] = {
  3, 5, 2, 7, 4, 10, 1, 9, 8, 6
};

unsigned char P8_TABLE[] = {
  6, 3, 7, 4, 8, 5, 10, 9 
};

#endif
