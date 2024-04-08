#ifndef HUFF_DECODE_H
#define HUFF_DECODE_H

#include "huff_const.h"

// Define la estructura del nodo de decodificación Huffman.
struct huff_decode_node {
	struct huff_decode_node* left;  
	struct huff_decode_node* right; 
	int is_leaf;                    // Indica si el nodo es una hoja.
	char symbol;                    // El símbolo que representa el nodo (en caso de ser una hoja).
};

// Nombre corto para la estructura de nodo de decodificación Huffman.
typedef struct huff_decode_node HDecodeNode;

// Función utilizada para crear un nodo de decodificación Huffman.
HDecodeNode* create_huff_decode_node();

// Función utilizada para liberar la memoria asignada al árbol de decodificación.
void free_huff_decode_tree(HDecodeNode* node);

// Función para construir un árbol de decodificación Huffman basado en el libro de códigos.
// El formato del libro de códigos debe ser como sigue:
// a 10000
// b 10001
// ...
// Para cada símbolo, crea nodos desde la parte superior hasta la inferior del árbol.
void build_huff_decode_tree(FILE* fp, HDecodeNode* root_decode);

// Función para decodificar.
// f_in es el puntero al archivo codificado.
// f_out es el puntero al archivo donde escribir el mensaje decodificado.
void huff_decode(FILE* f_in, FILE* f_out, HDecodeNode* root_decode);

// Interfaz para decodificar un archivo.
int huff_decode_file(const char* filename, const char* codebook_filename, const char* decoded_filename);

#endif // HUFF_DECODE_H
