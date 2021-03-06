#ifndef CACHEMEMORY_H
#define CACHEMEMORY_H
#include "CacheSet.h"
#include <vector>

class MemoryBus;

class CacheMemory{
	public:
		// Tipo de datos para las direcciones de memoria.
		typedef unsigned long dir_t;

		CacheMemory();
		CacheMemory(const dir_t &v_mem_size, const dir_t &v_block_size);
		~CacheMemory();
		void set_block_size(const dir_t&);
		//void set_assoc(const dir_t&);
		void set_mem_size(const dir_t&);
		const void print();
		char getState(const dir_t&);


		/* La estrutura de datos para las posiciones de memoria del cache sera
		   un arreglo de bloques, tal que el index del array es el mismo que el del set
		   Luego cada set consiste en un bloque de cache, cada uno incluye
		   el respectivo tag, e informacion de estado.
		   Asi, el tipo de datos sera un std::array< CacheSet >

		*/
		std::vector< Block > set_vec;
		
		/**
		 * Operaciones sobre los datos del cache
		 */
		bool read(const dir_t&);
		bool write(const dir_t&);
		void decode_dir(const dir_t&, dir_t&, dir_t&);
		dir_t encode_dir(const dir_t&, const dir_t&);
		bool snoop(const dir_t&);
		bool invalidate(const dir_t&);

		/**
		 * Operaciones sobre el bus
		 */
		int read_miss(const dir_t&);
		int write_miss(const dir_t&);
		void write_back(const dir_t&);
		int invalidate_broadcast(const dir_t&);
		void setBus(MemoryBus *const);
	private:
		dir_t block_size;
		dir_t mem_size;

		int tag_size;
		int index_size;
		int offset_size;
		dir_t block_num;

		dir_t my_mask;

		// Bus de memoria inferior
		MemoryBus *bus;

		// Tamano de las direcciones de memoria
		const static int DIR_SIZE = 32;

		//Block* find(const dir_t&);
		void initialize();
		const bool check_pow2(const dir_t&);
		//void set_tag();
};



#endif
