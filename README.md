## Procesador Huffman

El Procesador Huffman es una herramienta de línea de comandos para la compresión y descompresión de archivos de texto, basada en el algoritmo de Huffman. Este documento proporciona las instrucciones necesarias para compilar y ejecutar la herramienta.
Requisitos previos

Para utilizar el Procesador Huffman, asegúrate de tener instalado GCC (GNU Compiler Collection) en tu sistema. Esto es necesario para compilar el código fuente del proyecto.
Compilación

Para compilar el Procesador Huffman, sigue estos pasos:

    Abre una terminal.
    Navega hasta el directorio que contiene los archivos fuente (main.c, huff_encode.c, huff_decode.c).
    Ejecuta el siguiente comando:

```
gcc -o huffman_processor main.c huff_encode.c huff_decode.c
```

Este comando compilará los archivos fuente y generará un ejecutable llamado huffman_processor.
Uso

El Procesador Huffman puede ejecutarse en dos modos: codificación y decodificación.
Codificación

Para codificar archivos de texto, utiliza el siguiente comando:

```
./huffman_processor -e <DirectorioLibros> <DirectorioComprimidos> <DirectorioCodebook>
```

    <DirectorioLibros>: Ruta al directorio que contiene los archivos de texto que deseas comprimir.
    <DirectorioComprimidos>: Ruta al directorio donde se guardarán los archivos resultantes de la compresión.
    <DirectorioCodebook>: Ruta al directorio donde se almacenarán los codebooks generados durante la compresión.


## Decodificación

Para decodificar archivos previamente comprimidos con el Procesador Huffman, utiliza el siguiente comando:

```
./huffman_processor -d <DirectorioComprimidos> <DirectorioDescomprimidos> <DirectorioCodebook>
```

    <DirectorioComprimidos>: Ruta al directorio que contiene los archivos comprimidos que deseas descomprimir.
    <DirectorioDescomprimidos>: Ruta al directorio donde se guardarán los archivos descomprimidos.
    <DirectorioCodebook>: Ruta al directorio que contiene los codebooks necesarios para la descompresión.
