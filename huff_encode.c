#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "huff_encode.h"

// Función para crear un nodo de codificación Huffman. Asigna memoria para el nodo.
HEncodeNode* create_huff_encode_node(char symbol, double freq, int is_leaf) {
    HEncodeNode* p_node = (HEncodeNode*)malloc(sizeof(HEncodeNode));
    if (p_node != NULL) {
        p_node->left = NULL;  // Inicializa el hijo izquierdo a NULL
        p_node->right = NULL; // Inicializa el hijo derecho a NULL
        p_node->parent = NULL; // Inicializa el padre a NULL
        p_node->next = NULL; // Inicializa el siguiente nodo en la cola de prioridad a NULL
        p_node->freq = freq; // Establece la frecuencia del nodo
        p_node->is_leaf = is_leaf; // Indica si el nodo es una hoja
        p_node->symbol = symbol; // Establece el símbolo del nodo
    }
    return p_node;
}

// Función para insertar un nodo en la cola de prioridad.
void insert_huff_pqueue(HEncodeNode* node, HEncodeNode** q_head) {
    printf("inserting node: (%c, %f)\n", node->symbol, node->freq);
    if (*q_head == NULL) { // Si la cola está vacía, el nodo se convierte en el nuevo cabeza.
        *q_head = node;
        return;
    }
    HEncodeNode* curr_node = NULL;
    HEncodeNode* prev_node = NULL;
    curr_node = *q_head;
    // Busca la posición correcta para el nodo según su frecuencia.
    while ((curr_node != NULL) && (curr_node->freq < node->freq)) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    // Inserta el nodo en la posición encontrada.
    if (curr_node == *q_head) {
        node->next = *q_head;
        *q_head = node;
    } else {
        prev_node->next = node;
        node->next = curr_node;
    }
}

// Función para mostrar la cola de prioridad.
void disp_huff_pqueue(HEncodeNode* q_head) {
    printf("priority queue: ");
    while (q_head) {
        printf("(%c, %f),", q_head->symbol, q_head->freq);
        q_head = q_head->next;
    }
    printf("\n");
}

// Función para extraer (pop) el elemento de mayor prioridad (menor frecuencia) de la cola.
HEncodeNode* pop_huff_pqueue(HEncodeNode** q_head) {
    if (*q_head == NULL) return NULL;
    HEncodeNode* p_node = *q_head;
    *q_head = (*q_head)->next;
    printf("popped: (%c, %f)\n", p_node->symbol, p_node->freq);
    return p_node;
}

// Función para generar el codebook de forma recursiva.
void generate_huff_codebook(HEncodeNode* root, int depth, char* codebook) {
    if (root->is_leaf) {
        int len = depth;
        char symbol = root->symbol;
        *(codebook + ((size_t)symbol) * HUFF_MAX_LEN + len) = 0; // Agrega un carácter nulo al final del string.
        HEncodeNode* parent = root->parent;
        // Recorre el camino desde la hoja hasta la raíz para generar el código.
        while (parent != NULL && len > 0) {
            if (root == parent->left) {
                *(codebook + ((size_t)symbol) * HUFF_MAX_LEN + (--len)) = '0';
            } else {
                *(codebook + ((size_t)symbol) * HUFF_MAX_LEN + (--len)) = '1';
            }
            root = parent;
            parent = root->parent;
        }
        printf("built code: (%c, %s)\n", symbol, codebook + ((size_t)symbol) * HUFF_MAX_LEN);
    } else {
        // Si no es una hoja, continúa generando el codebook para sus hijos.
        generate_huff_codebook(root->left, depth + 1, codebook);
        generate_huff_codebook(root->right, depth + 1, codebook);
    }
}

// Función para escribir el codebook en un archivo.
void write_huff_codebook(FILE* f_out, char* codebook) {
    int i = 0;
    for (i = 0; i < HUFF_MAX_SYMBOLS; i++) {
        if (*(codebook + i * HUFF_MAX_LEN)) {
            fprintf(f_out, "#%c %s\n", i, codebook + i * HUFF_MAX_LEN);
        }
    }
}

// Función para escribir el flujo de bits codificados en un archivo.
void write_huff_encode_stream(FILE* f_out, char* str, char* codebook) {
    while (*str) {
        fprintf(f_out, "%s", codebook + (size_t)(*str) * HUFF_MAX_LEN);
        str++;
    }
}

// Función para liberar la memoria del árbol de codificación Huffman.
void free_huff_encode_tree(HEncodeNode* root) {
    if (root == NULL) return;
    free_huff_encode_tree(root->left);
    free_huff_encode_tree(root->right);
    free(root);
}

// Función para construir el árbol de codificación Huffman a partir de un arreglo de símbolos y sus frecuencias.
void build_huff_encode_tree(const char* str_arr, const double* freq_arr, size_t len, HEncodeNode** q_head) {
    size_t i = 0;
    HEncodeNode* left = NULL;
    HEncodeNode* right = NULL;
    HEncodeNode* parent = NULL;
    // Inserta todos los elementos en la cola de prioridad.
    for (i = 0; i < len; i++) {
        insert_huff_pqueue(create_huff_encode_node(str_arr[i], freq_arr[i], 1), q_head);
    }
    // Construye el árbol combinando los dos nodos de menor frecuencia hasta que solo queda uno.
    for (i = 0; i < len - 1; i++) {
        left = pop_huff_pqueue(q_head);
        right = pop_huff_pqueue(q_head);
        parent = create_huff_encode_node(0, left->freq + right->freq, 0);
        parent->left = left;
        parent->right = right;
        left->parent = parent;
        right->parent = parent;
        insert_huff_pqueue(parent, q_head);
    }
}

// Función para construir el árbol de codificación Huffman utilizando solo un arreglo de frecuencias.
void build_huff_encode_tree256(double* freq_arr, size_t len, HEncodeNode** q_head) {
    assert(len == 256 && 256 <= HUFF_MAX_SYMBOLS);
    size_t i = 0;
    HEncodeNode* left = NULL;
    HEncodeNode* right = NULL;
    HEncodeNode* parent = NULL;
    size_t valid_char_num = 0;
    // Inserta todos los caracteres válidos en la cola de prioridad.
    for (i = 0; i < len; i++) {
        if (freq_arr[i] > 0.0f) {
            insert_huff_pqueue(create_huff_encode_node((char)i, freq_arr[i], 1), q_head);
            valid_char_num++;
        }
    }
    // Construye el árbol combinando nodos de la cola.
    for (i = 0; i < valid_char_num - 1; i++) {
        left = pop_huff_pqueue(q_head);
        right = pop_huff_pqueue(q_head);
        parent = create_huff_encode_node(0, left->freq + right->freq, 0);
        parent->left = left;
        parent->right = right;
        left->parent = parent;
        right->parent = parent;
        insert_huff_pqueue(parent, q_head);
    }
}

// Función para escribir el archivo codificado leyendo del archivo original.
void write_huff_encode_stream_from_file(FILE* f_in, FILE* f_out, char* codebook) {
    char c;
    unsigned char buffer = 0;
    int bit_count = 0;
    // Lee cada carácter del archivo de entrada, busca su código en el codebook y escribe el flujo de bits en el archivo de salida.
    while ((c = getc(f_in)) != EOF) {
        char* code = codebook + (size_t)c * HUFF_MAX_LEN;
        while (*code) {
            buffer = (buffer << 1) | (*code - '0'); // Convierte el código de string a bits.
            bit_count++;
            if (bit_count == 8) { // Cuando el buffer está lleno, escribe en el archivo de salida.
                fwrite(&buffer, sizeof(buffer), 1, f_out);
                buffer = 0;
                bit_count = 0;
            }
            code++;
        }
    }
    // Escribe los bits restantes en el archivo de salida.
    if (bit_count > 0) {
        buffer <<= (8 - bit_count); // Alinea los bits a la derecha.
        fwrite(&buffer, sizeof(buffer), 1, f_out);
    }
}

// Función para contar los caracteres ASCII en el archivo de entrada y calcular su frecuencia.
void huff_count_char(double* freq_arr, FILE* f_in, size_t len) {
    assert(len == 256 && len <= HUFF_MAX_SYMBOLS);
    char c;
    double s = 0.0f; // Suma total de caracteres.
    // Cuenta cada carácter y suma al total.
    while ((c = getc(f_in)) != EOF) {
        freq_arr[(size_t)c] += 1.0f;
        s += 1.0f;
    }
    // Calcula la frecuencia de cada carácter.
    size_t i = 0;
    for (i = 0; i < len; i++) {
        if (freq_arr[i] > 0.0f) {
            freq_arr[i] = freq_arr[i] / s;
        }
    }
}

// Función interfaz para codificar un archivo. Genera el archivo codificado y el codebook.
int huff_encode_file(const char* filename, const char* encoded_filename, const char* codebooks_dir) {
    FILE* f_in = fopen(filename, "r");
    if (f_in == NULL) {
        printf("Cannot open %s\n.exit.\n", filename);
        return -1;
    }
    double freq_arr[HUFF_MAX_SYMBOLS];
    for (int i = 0; i < HUFF_MAX_SYMBOLS; i++) {
        freq_arr[i] = 0.0f; // Inicializa el arreglo de frecuencias.
    }
    huff_count_char(freq_arr, f_in, HUFF_MAX_SYMBOLS); // Cuenta y calcula la frecuencia de los caracteres.
    fclose(f_in);
    HEncodeNode* q_head = NULL;
    HEncodeNode* root_encode = NULL;
    char codebook[HUFF_MAX_SYMBOLS][HUFF_MAX_LEN];
    memset(codebook, 0, sizeof(codebook)); // Inicializa el codebook.
    build_huff_encode_tree256(freq_arr, HUFF_MAX_SYMBOLS, &q_head); // Construye el árbol de codificación.
    root_encode = pop_huff_pqueue(&q_head); // Extrae la raíz del árbol.
    generate_huff_codebook(root_encode, 0, &codebook[0][0]); // Genera el codebook.
    // Genera el nombre del archivo del codebook y lo escribe.
    char* baseName = strrchr(filename, '/');
    baseName = baseName ? baseName + 1 : (char*)filename;
    char codebookFilename[1024];
    snprintf(codebookFilename, sizeof(codebookFilename), "%s/%s_codebook.txt", codebooks_dir, baseName);
    FILE* f_out = fopen(codebookFilename, "w");
    if (f_out == NULL) {
        printf("Cannot open %s\n.exit.\n", codebookFilename);
        return -1;
    }
    write_huff_codebook(f_out, &codebook[0][0]);
    fclose(f_out);
    // Abre nuevamente el archivo original y el archivo codificado para escribir el flujo de bits.
    f_in = fopen(filename, "r");
    if (f_in == NULL) {
        printf("Cannot open %s for a second time.\n.exit.\n", filename);
        return -1;
    }
    f_out = fopen(encoded_filename, "wb");
    if (f_out == NULL) {
        printf("Cannot open %s\n.exit.\n", encoded_filename);
        fclose(f_in); // Asegura cerrar el archivo de entrada antes de salir.
        return -1;
    }
    write_huff_encode_stream_from_file(f_in, f_out, &codebook[0][0]); // Escribe el flujo de bits codificado.
    fclose(f_in);
    fclose(f_out);
    free_huff_encode_tree(root_encode); // Libera la memoria del árbol de codificación.
    return 0;
}
