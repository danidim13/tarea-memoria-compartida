#include "MemoryBus.h"

MemoryBus::MemoryBus():lower_mem(0){

}

MemoryBus::~MemoryBus(){

}

bool MemoryBus::add_higher(CacheMemory *const memory){
	return higher_mem.insert(memory).second;
}

bool MemoryBus::del_higher(CacheMemory *const memory){
	return (bool) higher_mem.erase(memory);
}

bool MemoryBus::add_lower(CacheMemory *const memory){
	if(!lower_mem){
		lower_mem = memory;
		return true;
	}
	return false;
}

bool MemoryBus::del_lower(CacheMemory *const memory){
	if(lower_mem == memory){
		lower_mem = 0;
		return true;
	}
	return false;
}

int MemoryBus::read_miss(const dir_t &addr, CacheMemory *const origin){
	std::set<CacheMemory*>::iterator it = higher_mem.find(origin);
	if(it == higher_mem.end()){
		std::cerr << "Error, read request from unexpected Cache" << std::endl;
		return -1;
	}
	int count = 0;

	// Busca en los caches del mismo nivel (snooping) por el dato;
	for(it = higher_mem.begin(); it != higher_mem.end(); it++){
		if(*it != origin){
			std::cout << "Read snooping..." << std::endl;
			count += (int) ((*it)->snoop(addr));
			if(count) std::cout << "Snooped!" << std::endl;
		}
	}
	// Busca en el cache de nivel inferior
	if(!count && lower_mem)
		lower_mem->read(addr);

	// count deberia ser 1 o 0, otro valor indica algun error.
	return count;
}

int MemoryBus::write_miss(const dir_t &addr, CacheMemory *const origin){
	std::set<CacheMemory*>::iterator it = higher_mem.find(origin);
	if(it == higher_mem.end()){
		std::cerr << "Error, write request from unexpected Cache" << std::endl;
		return -1;
	}
	int count = 0;

	// Busca en los caches del mismo nivel (snooping) por el dato;
	for(it = higher_mem.begin(); it != higher_mem.end(); it++){
		if(*it != origin){
			std::cout << "Write snooping..." << std::endl;
			count += (int) ((*it)->snoop(addr));
			if(count) std::cout << "Snooped!" << std::endl;
		}
	}
	// Busca en el cache de nivel inferior
	if(!count && lower_mem)
		lower_mem->read(addr);

	// count deberia ser 1 o 0, otro valor indica algun error.
	return count;
}

int MemoryBus::invalidate_broadcast(const dir_t &addr, CacheMemory *const origin){
	std::set<CacheMemory*>::iterator it = higher_mem.find(origin);
	if(it == higher_mem.end()){
		std::cerr << "Error, invalidate request from unexpected Cache" << std::endl;
		return -1;
	}
	
	int count = 0;
	// Invalida en los caches del mismo nivel
	for(it = higher_mem.begin(); it != higher_mem.end(); it++){
		if(*it != origin){
			std::cout << "Invalidate snooping..." << std::endl;
			count += (int) ((*it)->invalidate(addr));
			if(count) std::cout << "Invalidated!" << std::endl;
		}
	}
	// Busca en el cache de nivel inferior
	if(lower_mem)
		count += (int) lower_mem->invalidate(addr);

	return count;
}

int MemoryBus::write_back(const dir_t &addr){
	if(lower_mem){
		return lower_mem->write(addr);
	}
	return -1;
}
		
