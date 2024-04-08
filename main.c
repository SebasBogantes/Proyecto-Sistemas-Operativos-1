#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "huff_encode.h"
#include "huff_decode.h" 

void ensure_directory_exists(const char* dir_path) {
    struct stat st = {0};
    
    if (stat(dir_path, &st) == -1) {
        mkdir(dir_path, 0700);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s -e|-d <input_directory> <output_directory> <codebooks_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int encode = 0;
    if (strcmp(argv[1], "-e") == 0) {
        encode = 1;
    } else if (strcmp(argv[1], "-d") == 0) {
        encode = 0;
    } else {
        printf("Invalid mode. Use -e for encode or -d for decode.\n");
        exit(EXIT_FAILURE);
    }

    const char* input_dir = argv[2];
    const char* output_dir = argv[3];
    const char* codebooks_dir = argv[4];

    ensure_directory_exists(output_dir);
    ensure_directory_exists(codebooks_dir);

    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(input_dir)) == NULL) {
        perror("Failed to open input directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char input_filepath[1024];
        snprintf(input_filepath, sizeof(input_filepath), "%s/%s", input_dir, entry->d_name);

        char output_filepath[1024];
        snprintf(output_filepath, sizeof(output_filepath), "%s/%s", output_dir, entry->d_name);

        if (encode) {
            if (huff_encode_file(input_filepath, output_filepath, codebooks_dir) != 0) {
                printf("Failed to encode file: %s\n", input_filepath);
            }
        } else {
            // Decodificación: Se ajusta para asegurar el codebook correcto.
            printf("Decoding file: %s\n", input_filepath);

            char codebookFilename[1024];
            // Se asume que los archivos codificados terminan en "_encoded"
            char* encodedPosition = strstr(entry->d_name, "_encoded");
            if (encodedPosition != NULL) {
                *encodedPosition = '\0'; // Corta el nombre para remover "_encoded".
            }
            snprintf(codebookFilename, sizeof(codebookFilename), "%s/%s_codebook.txt", codebooks_dir, entry->d_name);

            if (huff_decode_file(input_filepath, codebookFilename, output_filepath) != 0) {
                printf("Failed to decode file: %s\n", input_filepath);
            }
        }
    }

    closedir(dir);
    return 0;
}
