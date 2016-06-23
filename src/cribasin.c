//---------------------------------------------------
//--------- PROGRAMA cribasin.c----------------------
//--Obtiene(imprime) los numeros primos desde
//--cero hasta "tam" utilizando el algoritmo
//--Criba de Eratostenes - Sin paralelizacion
//---------------------------------------------------
#include <stdio.h>
#include "stdbool.h"
#include <time.h>
//#include "mpi.h"

void criba(bool m[], int tam){
    int k=0;
    m[0] = false;
    m[1] = false;
    // Llena la matriz m[] de unos
    for(int i = 2; i <= tam; ++i){
      m[i] = true;
    }
    //Lazo principal del algoritmo
    //Pone en 0 cada casilla de la matriz m[] que correspnoda a un numero no primo
    for(int i = 2; i*i <= tam; ++i) {
        if(m[i]) {
            for(int h = 2; i*h <= tam; ++h)
                m[i*h] = false;
        }
    }
    //Imprime los numeros primos (aquellos donde m[i] = 1)
    for(int i = 0; i<tam; i++){
      if(m[i]){
        k=k+1;
        printf("Primo %d: %d \n",k,i);  
      }
    }
}

int main(){
  int tam = 5000;
  bool m[tam];
//-------------------Timing----------------
  clock_t begin, end;
  double time_spent;
  begin = clock();
//-----------------------------------------
  criba(m,tam);
//-------------------Timing----------------
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time spent %f\n",time_spent );
//-----------------------------------------
}
