#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> 
#include <stdbool.h>
#include "sdes_tables.h"

uint16_t tipo8 = 0x0080, tipo16=0x8000;

void prueba(uint16_t word, uint8_t tipo, uint8_t fin){
    //definicionn de las variables
    uint16_t cont; 
    // configuracion respecto del tipo de variable
    if (tipo == tipo8){
        cont = 0x0080; // 0000 0000 1000 0000
        // fin = 0;
    }else{
        cont = 0x8000;
        fin = 63;
    }

    for (cont;cont>fin;cont>>=1){
        if(cont & word){
            printf("1");
        }else{
            printf("0");
        }
    }
    printf("\n\n");
}


void permutacion8(uint8_t *word, unsigned char tabla[], int tam){
    uint8_t word_src, word_f;
    uint8_t mask;
    word_f=*word;
        for(int i = 1; i <= tam;i++){
            word_src = *word;
            mask = 128;           
            int lugar = tabla[i-1]; //leemos posicion
            mask >>= (lugar - 1); // mascara bit a extraer extraer
            word_src &= mask; //extraemos el valor de bit
            if((lugar-i) > 0){
                word_src <<= (lugar-i);
                mask = 128  >> i-1;               
            }else{
                word_src >>= (i-lugar);
                mask = 128  >> i-1;
            }
            //sustitucion de bit
            if(word_src==0){
                word_f &= ~(mask);
            }else{
                word_f |= word_src;
            } 
        }
        *word = word_f;
}

void permutacion16(uint16_t *word, unsigned char tabla[], int tam){
    uint16_t word_src, word_f;
    uint16_t maskG, mask;
    word_f=*word;
    maskG = 0x8000;// 10000 0000 0000 0000 
        for(int i = 1; i <= tam;i++){
            word_src = *word;
            mask = maskG;         
            int lugar = tabla[i-1]; //leemos posicion
            mask >>= (lugar - 1); // mascara bit a extraer extraer
            word_src &= mask; //extraemos el valor de bit
            if((lugar-i) > 0){
                word_src <<= (lugar-i);
                mask = maskG  >> i-1;               
            }else{
                word_src >>= (i-lugar);
                mask = maskG  >> i-1;
            }
            //sustitucion de bit
            if(word_src==0){
                word_f &= ~(mask);
            }else{
                word_f |= word_src;
            } 
        }
        *word = word_f;
}

void LeftShift(uint16_t *clave, int opt){
    // variables
    uint16_t bitL, bitR;
    uint16_t L = (*clave & 0xF800);  // 0b1111 1000 0000 0000
    uint16_t R = (*clave & 0x07C0);  // 0b0000 0111 1100 0000
    
    if (opt == 1){                   //realizamos el LS-1
        bitL = 0x8000;               // 1000 0000 ......
        bitR = 0x0400;               // 0000 0100 ......
    }else{                           //realizamos el LS2
        bitL = 0xC000;
        bitR = 0x0600;
    }

    bitL &= *clave;
    bitL >>= (5-opt);
    L = (L << opt) | bitL;
    bitR &= R; 
    bitR >>= (5-opt);           // desplazo el bit a posicion 10
    R = (R << opt) | bitR; 
    R <<= 5;
    R >>= 5;

    *clave = R | L;             // Devolvemos la clave desplazada
}

void cajas_s(uint8_t word, uint8_t *S){
    
    //variables
    uint8_t fila, columna, S0, S1, bit, mask_S0 = 0xF0, mask_S1 = 0x0F; // mascara S0 & mascara S1

    mask_S0 &= word; 
    fila = (0x90 & mask_S0) >> 4;
    fila = (((0x08 & fila) >> 2) | fila) & 0x03 ;
    columna = (0x60 & mask_S0) >> 5;
    S0 = S_GATES_TABLE[0][fila][columna];

    mask_S1 &= word; 
    fila = 0x09 & mask_S1;
    fila = (((0x08 & fila) >> 2) | fila) & 0x03;
    columna = (0x06 & mask_S1) >> 1;
    S1 = S_GATES_TABLE[1][fila][columna];
    *S = (S0<<6) | (S1<<4);    //unimos S0 Y S1
}


int main(int argc, char *argv[]){
    uint8_t word, word_aux, key, Lw, Rw, K1, K2, tam, word_XOR, S, fk1, fk2, fk;
    uint16_t Lc, Rc, clave = 0x0000, maskKey = 0x8000, clave_aux;
    bool decrypt;
    int opt;

    FILE *keyFile;
    FILE *inFile;
    FILE *outFile;

    while((opt=getopt(argc, argv, "ed::k:")) != -1){
        switch(opt){
            case 'e':
                if ((inFile = fopen((char * )argv[2], "rb"))==NULL){
                    printf("Error al abrir el fichero a encriptar");
                    exit(1);
                }
                if ((outFile = fopen((char * )argv[3], "wb"))==NULL){
                    printf("Error al crear archivo encriptado");
                    exit(2);
                }
                decrypt = false;
            break;
            case 'd':
                if ((inFile = fopen((char * )argv[2], "rb"))==NULL){
                    printf("Error al abrir el archivo encriptado");
                    exit(1);
                }
                if ((outFile = fopen((char * )argv[3], "wb"))==NULL){
                    printf("Error al abrir el archivo des-ecriptado");
                    exit(2);
                }
                decrypt = true;
            break;
            case 'k':
                if ((keyFile = fopen((char * )argv[5], "rb"))==NULL){
                    printf("Error al abrir el fichero con la contrasena");
                    exit(3);
                }
            break;                        
        }
    }
    // Lectura de la clave
    while(fread(&key, sizeof key, 1, keyFile)==1){
        if (key==0x31){
            clave |= maskKey;
        }
        maskKey >>= 1;
    }
    fclose(keyFile);
    printf("\nclave de entrada: ");
    prueba(clave, tipo16, 63);


    // Obtencion de las Subclaves K1 y K2
    permutacion16(&clave, P10_TABLE, sizeof(P10_TABLE));                    // permutacion de la clave P10
    LeftShift(&clave, 1);                                                   // desplazamiento de la clave LS-1               
    clave_aux = clave;                                                      // utilizamos una var. auxiliar ya que necesitaremos clave10 para K2
    permutacion16(&clave_aux, P8_TABLE, sizeof(P8_TABLE));                  // Aplicamo la permutacion P8
    K1 = clave_aux >> 8;                                                    // Obtenemos K1

    LeftShift(&clave, 2);                                                   // desplazamiento LS-2                
    permutacion16(&clave, P8_TABLE, sizeof(P8_TABLE));                      // Aplica la permutacion P8
    K2 = clave >> 8;                                                        // Obtenemos K2

    if(decrypt == true){                                                    // si des-encriptamos intercambiamos K1 y K2
        key = K1;
        K1 = K2;
        K2 = key;
    }

    printf("\nK1 = ");
    prueba(K1, tipo8, 0);
    printf("\nK1 = ");
    prueba(K1, tipo8, 0);

    //en cualquier caso, encriptamos.
    while(fread(&word, sizeof word, 1, inFile)==1){
        printf("\nDato de entrada 0x%.2x, %c: ", word, word);
        tam = sizeof(IP_TABLE);                                             // Numero de cambios
        permutacion8(&word, IP_TABLE, tam);                                 // Aplicamos la Permutacion IP
        Lw = word & 0xF0;                                                   //extraigo L
        Rw = (word & 0X0F) << 4;                                            //extraigo R
        word_aux = Rw;                                                      // Utilizamos una variables auxiliar para recicar mas adelante Rw
        permutacion8(&word_aux, EXPANSION_TABLE, sizeof(EXPANSION_TABLE));  // Aplicamos la expansion sobre R
        word_XOR = (word_aux ^ K1);                                         // Aplicamos la XOR con K1
        cajas_s(word_XOR, &S);                                              // Aplicamos las cajas S
        permutacion8(&S, P4_TABLE, sizeof(P4_TABLE));                       // Aplicamos la tabla permutacion P4
        fk1 = (S ^ Lw);                                                     // fk1 

        // procedemos a realizar fk2 
        Lw = Rw;
        Rw = fk1;
        tam = sizeof(EXPANSION_TABLE);                 
        permutacion8(&Rw, EXPANSION_TABLE, sizeof(EXPANSION_TABLE));        // Aplicamos la expansion sobre R
        word_XOR = (Rw ^ K2);                                               // Aplicamos la XOR con K2
        cajas_s(word_XOR, &S);                                              // Aplicamos cajas S
        permutacion8(&S, P4_TABLE, sizeof(P4_TABLE));                       // Aplicamos la Permutacion P4
        fk2 = (S ^ Lw);                                                     // fk2
        fk = fk2 | (fk1 >> 4);                                              // fk, union de los 4 bits de fk2 y fk1
        permutacion8(&fk, IP_INV_TABLE, sizeof(IP_INV_TABLE));              // Salida des/encriptada con permutacion final IP_inversa
        printf("Dato encriptado 0x%.2x ", fk);
        fwrite(&fk, sizeof fk, 1, outFile);                                 //almacen de Byte des/encriptado en fichero de salida
    }
    
    fclose(inFile);
    fclose(outFile);
    
    return 0;
}
