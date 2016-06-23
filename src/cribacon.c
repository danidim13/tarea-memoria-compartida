//---------------------------------------------------
//--------- PROGRAMA cribacon.c----------------------
//--Obtiene(imprime) los numeros primos desde
//--cero hasta "tam" utilizando el algoritmo
//--Criba de Eratostenes - Con paralelizacion
//---------------------------------------------------
#include <stdio.h>
#include "mpi.h"
#include "stdbool.h"
#include <time.h>

void criba(int m[], int tam, int id){
    int k=0;
    int n[tam];
    m[0] = 0;
    m[1] = 0;
    // Llena la matriz m[] de unos
    for(int i = 2; i <= tam; ++i){
      m[i] = 1;
    }
    //El lazo principal del algoritmo se divide en dos
    //cada uno de los dos procesos ejecuta una parte del lazo.
  if(id==0){
    for(int i = 2; i < 10; i=i+1) {
        if(m[i]==1) {
            for(int h = 2; i*h <= tam; ++h)
                m[i*h] = 0;
        }
    }
  }
  if(id==1){
    for(int i = 10; i*i <= tam; i=i+1) {
        if(m[i]==1) {
            for(int h = 2; i*h <= tam; ++h)
                m[i*h] = 0;
        }
    }
  }
  //Se unen los resultados utilizando la funcion MPI_Reduce
  //Esta funcion obtiene el valor minimo al comparar cada casilla de los arrays
  //en ambos procesos. El array resultante lo guarda en un array nuevo (n) del proceso 0.
  MPI_Reduce(m, n, tam, MPI_INT, MPI_MIN,0,MPI_COMM_WORLD);
  //El proceso 0 imprime los numeros primos
    if (id==0){
      for(int i = 0; i<tam; i++){
        if(n[i]){
          k=k+1;
          printf("Primo %d: %d \n",k,i);
        }
      }
    }
}

int main(){
  clock_t begin, end;
  double time_spent;
  int inicio,fin;
  int tam = 5000;
  int m[tam];
  // Inicializar MPI
  MPI_Init(NULL, NULL);

  //Obtener el numero de procesos
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  //Obtener el Rank del proceso
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  //-------------------Timing----------------
  if(world_rank==0){
    begin = clock();
  }
  //-----------------------------------------
  criba(m,tam,world_rank);
  MPI_Finalize();
  //-------------------Timing----------------
  if(world_rank==0){
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time spent %f\n",time_spent );
  }
  //-----------------------------------------
}
