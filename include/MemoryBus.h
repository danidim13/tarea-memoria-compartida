#ifndef MEMORYBUS_H
#define MEMORYBUS_H
#include "CacheMemory.h"
#include <set>

class MemoryBus{
	public:
		typedef unsigned long dir_t;

		MemoryBus();
		~MemoryBus();
		
		///////////////////////////////////////////////////////////
		// Manejo de Referencias a caches superiores e inferiores//
		
		// Return false si ya existia
		bool add_higher(CacheMemory *const memory);

		// Return false si no existia
		bool del_higher(CacheMemory *const memory);

		// Return false si ya existia 
		bool add_lower(CacheMemory *const memory);

		// Return false si no existia
		bool del_lower(CacheMemory *const memory);

		// Borrar todas las referencias
		void clear();
		
		///////////////////////////////////////////////////////////
	       	
		/**
		 * Operaciones y señales del bus de datos
		 *
		 */
		int read_miss(const dir_t& addr, CacheMemory *const origin);
		int write_miss(const dir_t& addr, CacheMemory *const origin);
		int invalidate_broadcast(const dir_t& addr, CacheMemory *const origin);
		int write_back(const dir_t& addr);
		

	private:
		std::set<CacheMemory*> higher_mem;
		CacheMemory *lower_mem;


};

#endif
