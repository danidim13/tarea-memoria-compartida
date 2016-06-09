#ifndef CACHESET_H
#define CACHESET_H
#include <iostream>

struct Block{
	Block():m_valid(false),m_tag(0){};
	~Block(){};
	//bool m_dirty;
	bool m_valid;
	unsigned long m_tag;

};

class CacheSet{
	public:
		CacheSet();
		CacheSet(int assoc);
		CacheSet(const CacheSet& other); // Constructor por copia
		~CacheSet();
		
		CacheSet& operator=(const CacheSet& rhs); // Asignacion

		typedef unsigned long dir_t;
		
		// Busca un bloque con el tag, si lo encuentra
		// devuelve true, de lo contrario devuelve false
		// y lo agrega en la posicion blocks[fifo]
		bool fetch(dir_t tag);
		void print();
		
	private: 
		Block *blocks;
		int fifo;
		int block_num;

		// Busca un bloque con el tag, devuelve
		// un puntero vacío si no encuentra
		Block *find(dir_t tag);
};

#endif
