#ifndef HUFF_ENCODE_H
#define HUFF_ENCODE_H

#include "huff_const.h"
#include <stdio.h>

// Estructura para los nodos del árbol de Huffman usados en la codificación.
struct huff_encode_node {
	struct huff_encode_node* left;   // Apunta al hijo izquierdo en el árbol; representa un '0'.
	struct huff_encode_node* right;  // Apunta al hijo derecho en el árbol; representa un '1'.
	struct huff_encode_node* parent; // Apunta al nodo padre en el árbol.
	struct huff_encode_node* next;   // Usado en la cola de prioridad.
	double freq;                     // Frecuencia del símbolo en el archivo.
	int    is_leaf;                  // Indica si el nodo es una hoja en el árbol.
	char   symbol;                   // El símbolo asignado a este nodo.
};

// Tipo definido para el nodo de codificación de Huffman.
typedef struct huff_encode_node HEncodeNode;

// Función para asignar memoria para un nodo de codificación de Huffman.
HEncodeNode* create_huff_encode_node(char symbol, double freq, int is_leaf);

// Función utilizada para insertar un nodo en la cola de prioridad.
void insert_huff_pqueue(HEncodeNode* node, HEncodeNode** q_head);

// Función utilizada para mostrar la cola de prioridad.
void disp_huff_pqueue(HEncodeNode* q_head);

// Función para extraer un elemento de la cola de prioridad.
HEncodeNode* pop_huff_pqueue(HEncodeNode** q_head);

// Función para generar el codebook de manera recursiva.
void generate_huff_codebook(HEncodeNode* root, int depth, char* codebook);

// Función para escribir el codebook en un archivo.
void write_huff_codebook(FILE* f_out, char* codebook);

// Función para escribir el flujo de bits codificados en un archivo.
void write_huff_encode_stream(FILE* f_out, char* str, char* codebook);

// Función para liberar la memoria del árbol de codificación Huffman.
void free_huff_encode_tree(HEncodeNode* root);

// Función para construir el árbol de codificación Huffman.
void build_huff_encode_tree(const char* str_arr, const double* freq_arr, size_t len, HEncodeNode** q_head);

// Función para construir el árbol de codificación Huffman solo con un arreglo de frecuencias.
void build_huff_encode_tree256(double* freq_arr, size_t len, HEncodeNode** q_head);

// Función para escribir el archivo codificado leyendo del archivo original.
void write_huff_encode_stream_from_file(FILE* f_in, FILE* f_out, char* codebook);

// Interfaz para codificar un archivo.
int huff_encode_file(const char* filename, const char* encoded_filename, const char* codebooks_dir);

// Función para contar caracteres ASCII.
void huff_count_char(double* freq_arr, FILE* f_in, size_t len);

#endif // !HUFF_ENCODE_H
