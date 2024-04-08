#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "huff_decode.h"

// Crea un nuevo nodo de decodificación Huffman.
HDecodeNode* create_huff_decode_node() {
    HDecodeNode* p_node = (HDecodeNode*)malloc(sizeof(HDecodeNode));
    if (p_node != NULL) {
        p_node->left = NULL;   // Hijo izquierdo.
        p_node->right = NULL;  // Hijo derecho.
        p_node->symbol = 0;    // Símbolo representado por el nodo.
        p_node->is_leaf = 0;   // Indica si es un nodo hoja.
    }
    return p_node;
}

// Libera la memoria del árbol de decodificación.
void free_huff_decode_tree(HDecodeNode* node) {
    if (node == NULL) return;
    free_huff_decode_tree(node->left);  // Libera el subárbol izquierdo.
    free_huff_decode_tree(node->right); // Libera el subárbol derecho.
    free(node);                         // Libera el nodo actual.
}

// Construye un árbol de decodificación basado en un libro de códigos.
void build_huff_decode_tree(FILE* fp, HDecodeNode* root_decode) {
    char symbol=0;
    char strcode[HUFF_MAX_LEN];
    int idx=0, length=0;
    int num_input=0;
    HDecodeNode* curr_node = NULL;
    while (!feof(fp)) {
        memset(strcode, 0, sizeof(strcode));
        symbol = 0;
        num_input = fscanf(fp, "#%c %s\n", &symbol, strcode); // Lee el símbolo y su código.
        if (num_input != 2) {
            break; // Finaliza si no hay más datos o hay un error.
        }
        curr_node = root_decode; // Comienza desde la raíz para este símbolo.
        length = strlen(strcode);
        for (idx = 0; idx < length; idx++) {
            if (strcode[idx] == '0') {
                // Si el carácter es '0', va o crea un nodo a la izquierda.
                if (curr_node->left == NULL) {
                    curr_node->left = create_huff_decode_node();
                }
                curr_node = curr_node->left;
            } else if (strcode[idx] == '1') {
                // Si el carácter es '1', va o crea un nodo a la derecha.
                if (curr_node->right == NULL) {
                    curr_node->right = create_huff_decode_node();
                }
                curr_node = curr_node->right;
            } else {
                printf("unexpected char %c\n", strcode[idx]);
                assert(0); // Falla si el código contiene caracteres inesperados.
            }
        }
        // Asigna el símbolo al último nodo como nodo hoja.
        curr_node->is_leaf = 1;
        curr_node->symbol = symbol;
    }
}

// Decodifica un archivo basado en el árbol de decodificación Huffman.
void huff_decode(FILE* f_in, FILE* f_out, HDecodeNode* root_decode) {
    unsigned char c = 0;
    int bit;
    HDecodeNode* curr_node = root_decode;
    while (fread(&c, sizeof(c), 1, f_in)) { // Lee el archivo codificado bit a bit.
        for (int i = 7; i >= 0; i--) {
            bit = (c >> i) & 1; // Obtiene el bit actual.
            if (bit == 0) curr_node = curr_node->left; // Va hacia la izquierda para '0'.
            else curr_node = curr_node->right;         // Va hacia la derecha para '1'.
            if (curr_node->is_leaf) { // Si llega a una hoja, escribe el símbolo.
                fprintf(f_out, "%c", curr_node->symbol);
                curr_node = root_decode; // Regresa a la raíz para el siguiente bit.
            }
        }
    }
}

// Interfaz para decodificar un archivo usando el libro de códigos especificado.
int huff_decode_file(const char* filename, const char* codebook_filename, const char* decoded_filename){
    FILE* f_in = fopen(codebook_filename, "rb");
    if(f_in == NULL){
        printf("cannot open %s\n.exit.\n", codebook_filename);
        return -1;
    }

    HDecodeNode* root_decode = create_huff_decode_node();
    build_huff_decode_tree(f_in, root_decode); // Construye el árbol de decodificación.
    fclose(f_in); // Cierra el archivo del libro de códigos.

    f_in = fopen(filename, "rb"); // Abre el archivo codificado.
    if(f_in == NULL){
        printf("cannot open %s\n.exit.\n", filename);
        return -1;
    }

    FILE* f_out = fopen(decoded_filename, "w"); // Abre el archivo de salida.
    if(f_out == NULL){
        printf("cannot open %s\n.exit.\n", decoded_filename);
        fclose(f_in); // Cierra el archivo de entrada antes de salir.
        return -1;
    }

    huff_decode(f_in, f_out, root_decode); // Decodifica el archivo.
    fclose(f_in); // Cierra el archivo de entrada.
    fclose(f_out); // Cierra el archivo de salida.
    free_huff_decode_tree(root_decode); // Libera la memoria del árbol.
    return 0;
}
