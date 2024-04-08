#ifndef HUFF_CONST_H
#define HUFF_CONST_H

// Define el número máximo de símbolos que se pueden manejar en el algoritmo Huffman.
// Esto es útil para limitar el rango de caracteres ASCII a 256, que es el tamaño total
// de caracteres ASCII extendidos que se pueden representar en un byte.
#ifndef HUFF_MAX_SYMBOLS
#define HUFF_MAX_SYMBOLS 256
#endif // !HUFF_MAX_SYMBOLS

// Define la longitud máxima de los códigos Huffman que se pueden generar.
// Esto es para asegurarse de que los códigos generados no excedan esta longitud,
// manteniendo así la eficiencia del algoritmo y evitando el desbordamiento de buffers.
#ifndef HUFF_MAX_LEN
#define HUFF_MAX_LEN 50
#endif // !HUFF_MAX_LEN

#endif // !HUFF_CONST_H
