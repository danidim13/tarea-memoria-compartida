# tarea-memoria-cache
Tarea Programada 1 para el curso
IE0521 - Estructuras de Computadores Digitales II

Desarrolado por Daniel Díaz y Gabriel Jiménez

Implementación de una memoria caché para un procesador con direcciones de 32 bits.

El archivo "aligned.trace" que contiene las direcciones accesadas durante la prueba no se incluye en el repositorio. Para que el programa funcione este debe encontrarse en el mismo directorio del repositorios. Se incluye un Makefile para la compilación del código, este usa G++, que es el compilador de C++ de GCC. Para compilar:

$ make

Si se desea realizar una compilación de cero:

$ make clean

$ make

El resultado de la compilación es un binario "Tarea.out", para correr una simulación:

$ ./Tarea.out <asociatividad> <tamaño del cache (en bytes)> <tamaño del bloque (en bytes)>

Por ejemplo si se quisiera simular un caché de 32 KB con bloques de 8 bytes y mapeo directo:

$ ./Tarea.out 1 32768 8

Para compilar y ejecutar los programas cribasin y cribacon (obtener numeros primos) se requieren las librerias OpenMPI y mpich2. 
Para instalar estas librerias, intente ejecutar los siguientes comandos:

apt-get install libopenmpi-dev
apt-get install mpich2