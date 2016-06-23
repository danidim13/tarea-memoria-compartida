#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "CacheMemory.h"
#include "MemoryBus.h"

// Lineas totales en aligned.trace
#define LINEAS 49642127

using namespace std;

void welcome();
bool checkArg(int, char*[], long long[]);

int main(int argc, char* argv[]){

	welcome();

	// Revision de argumentos, si hay errores el programa termina y retorna 1
	//long long parametros[2];
	//if(!checkArg(argc, argv, parametros))
	//	return 1;

	CacheMemory CPU0(8*1024, 16);
	CacheMemory CPU1(8*1024, 16);
	CacheMemory L2(64*1024, 16);
	MemoryBus conexion;

	CPU0.setBus(&conexion);
	CPU1.setBus(&conexion);
	conexion.add_lower(&L2);

	cout << "CPU0 L1 $:" << endl;
	CPU0.print();
	cout << "\nCPU1 L1 $:" << endl;
	CPU1.print();
	cout << "\nL2 $:" << endl;
	L2.print();
	cout << endl;

	ifstream datos("aligned.trace");
	string linea;
	char tipo;
	string dir_str;
	unsigned long dir_dec;
	bool hit_miss;
	long long cont_total = 0;
	long long cont_hits0 = 0;
	long long cont_misses0 = 0;
	long long cont_hits1 = 0;
	long long cont_misses1 = 0;
	long long i = 0;
	bool impar = true;


	unsigned long ultimas[20];
	while (getline(datos,linea).good() && linea.length()) {
		tipo = linea.at(linea.length()-1);
		
		//cout << i << ": " << linea;

		linea.erase(linea.begin()+8,linea.end());
		//Convierte la dirección a decimal
		std::istringstream(linea) >> std::hex >> dir_dec;
		if (LINEAS - i < 20) {
			ultimas[LINEAS-i] = dir_dec;
		}

		if (impar) {
			hit_miss = tipo == 'L' ? CPU0.read(dir_dec) : CPU0.write(dir_dec);
			cont_hits0 += hit_miss;
			cont_misses0 += !hit_miss;
		}
		else {
			hit_miss = tipo == 'L' ? CPU1.read(dir_dec) : CPU1.write(dir_dec);
			cont_hits1 += hit_miss;
			cont_misses1 += !hit_miss;
		}

		impar = !impar;
		cont_total++;
		//memoria.print();
		i++;
	}

	double miss_rate0, miss_rate1;

	miss_rate0 = ((double)cont_misses0 / (double) (cont_misses0 + cont_hits0) )*100;
	miss_rate1 = ((double)cont_misses1 / (double) (cont_misses1 + cont_hits1) )*100;

	cout << "CPU0:" << endl;
	cout << "Hits: " << cont_hits0 << endl;
	cout << "Misses: "<< cont_misses0 << endl;
	cout << "Miss Rate (porcentaje) :" << miss_rate0 << endl;

	cout << "CPU1:" << endl;
	cout << "Hits: " << cont_hits1 << endl;
	cout << "Misses: "<< cont_misses1 << endl;
	cout << "Miss Rate (porcentaje) :" << miss_rate1 << endl;

	cout << "\nTotal de instrucciones: " << cont_total << endl;


	cout << endl;
	cout << "Estado final de la memoria:" << endl;

	cout << "Dirección \tCPU0   CPU1   L2" << endl;
	for (int i = 19; i >= 0; i--) {
		cout << hex << ultimas[i] << " \t" << 
			CPU0.getState(ultimas[i]) << "      " <<
			CPU1.getState(ultimas[i]) << "      " <<
			L2.getState(ultimas[i])   << endl;
	}
	cout << "\nFin del programa" << endl;
	return 0;
}

void welcome(){
	//string mensaje;
	//string integrantes;
	cout << "Bienvenido a este programa!\n"
		 << "Simulador de una memoria cache\n"
		 << "Hecho por Daniel Diaz y Gabriel Jimenez\n"
		 << endl;

}

bool checkArg(int argc, char* argv[], long long parametros[]){
	if(argc != 3){
		cerr << "Error de argumentos, uso:\n"
			 << "\t " << argv[0] << " <tamano del cache> <tamano del bloque>"
			 << endl;
		return false;
	}
	parametros[0]= atoll(argv[1]);
	parametros[1]= atoll(argv[2]);
	return true;

}



