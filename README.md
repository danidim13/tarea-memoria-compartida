# tarea-memoria-cache
Tarea Programada 1 para el curso
IE0521 - Estructuras de Computadores Digitales II

Desarrolado por Daniel Díaz y Gabriel Jiménez

Implementación de protocolo MESI para memoria caché en un procesador con direcciones de 32 bits.

El archivo "aligned.trace" que contiene las direcciones accesadas durante la prueba no se incluye en el repositorio. Para que el programa funcione este debe encontrarse en el mismo directorio del repositorio. Se incluye un Makefile para la compilación del código, este usa G++, que es el compilador de C++ de GCC. Para compilar:

$ make MESI

Si se desea realizar una compilación de cero:

$ make clean

$ make MESI

El resultado de la compilación es un binario "MESI", para correr una simulación:

$ ./MESI

Para compilar y ejecutar los programas cribasin y cribacon (obtener numeros primos) se requieren las librerias OpenMPI y mpich2. 
Para instalar estas librerias, intente los siguientes comandos:


apt-get install libopenmpi-dev
apt-get install mpich2

Esto tambien se puede hacer con el makefile:


$ make deps

Para compilar:


$ make Criba

Para ejecutar el programa CRIBASIN, utilice el siguiente comando en la carpeta de ejecutables:

./cribasin

Para ejecutar el programa CRIBACON, utilice el siguiente comando en la carpeta de ejecutables:

mpirun -np 2 ./cribacon


Tambien se pueden compilar todos los programas al mismo tiempo con alguno de estos dos comandos:

$ make all

$ make

