#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "CacheMemory.h"


using namespace std;

void welcome();
bool checkArg(int, char*[], long long[]);

int main(int argc, char* argv[]){

	welcome();

	// Revision de argumentos, si hay errores el programa termina y retorna 1
	long long parametros[3];
	if(!checkArg(argc, argv, parametros))
		return 1;

	CacheMemory memoria(parametros[0], parametros[1], parametros[2]);
	memoria.print();

	ifstream datos("aligned.trace");
	string linea;
	unsigned long dir_dec;
	bool hit_miss;
	long long cont_total = 0;
	long long cont_hits = 0;
	long long cont_misses = 0;
	double miss_rate;

	while ( getline (datos,linea) ) {
//	while ( cont_total <= 10) {
		//Obtiene una dirección (una línea del archivo)
		getline(datos, linea);
		//Deja solo la dirección y borra el resto
		linea.erase(linea.begin()+8,linea.end());
		//Convierte la dirección a decimal
		std::istringstream(linea) >> std::hex >> dir_dec;
		hit_miss = memoria.fetch(dir_dec);
		if (hit_miss){
			cont_hits = cont_hits + 1;
		}
		cont_total = cont_total + 1;
		//memoria.print();
	}
	cont_misses = cont_total - cont_hits;
	miss_rate = ((double)cont_misses / (double)cont_total)*100.0;

	cout << "Hits: " << cont_hits << endl;
	cout << "Misses: "<< cont_misses << endl;
	cout << "Total: " << cont_total << endl;

	cout << "Miss Rate (porcentaje) :" << miss_rate << endl;

	cout << "fin del programa" << endl;
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
	if(argc != 4){
		cerr << "Error de argumentos, uso:\n"
			 << "\t " << argv[0] << " <asociatividad> <tamano del cache> <tamano del bloque>"
			 << endl;
		return false;
	}
	parametros[0]= atoll(argv[1]);
	parametros[1]= atoll(argv[2]);
	parametros[2]= atoll(argv[3]);
	return true;

}



