#include "CacheMemory.h"
#include "CacheSet.h"
#include "MemoryBus.h"
#include <sstream>
#include <cmath>
#include <iostream>

CacheMemory::CacheMemory(){
	block_size = 0;
        //assoc = 0;
        mem_size = 0;
	bus = 0;

        tag_size = 0;
        index_size = 0;
        offset_size = 0;
        block_num = 0;
}

CacheMemory::CacheMemory(const dir_t &v_mem_size, const dir_t &v_block_size):bus(0){
	//df
	//set_assoc(v_assoc);
	set_block_size(v_block_size);
	set_mem_size(v_mem_size);
	initialize();
}

CacheMemory::~CacheMemory(){
	/*if(bus){
		bus->del_higher(this);
	}*/
}

void CacheMemory::initialize(){
	// Cantidad de bloques y sets
	block_num = mem_size/block_size;

	// Se determinan los bits del offset
	offset_size = (int) log2(block_size);
	index_size = (int) log2(block_num);
	tag_size = DIR_SIZE - (offset_size + index_size);

	set_vec.resize(block_num, Block());

	my_mask = 0;
	for(int im = 0; im < index_size; im++){
		my_mask <<= 1;
		my_mask |= 1;
	}
}

void CacheMemory::set_block_size(const dir_t &val){
	if (check_pow2(val))
		block_size = val;
	else {
		std::cerr << "Error, block_size not valid" << std::endl;
		block_size = 1;
	}
}

void CacheMemory::set_mem_size(const dir_t &val){
	if (check_pow2(val) && val >= block_size)
		mem_size = val;
	else{
		std::cerr << "Error, mem_size not valid" << std::endl;
		mem_size = block_size;
	}
}

const bool CacheMemory::check_pow2(const dir_t &val){
	if(val){
		return (val & (val-1)) == 0;
	}
	return false;
}
const void CacheMemory::print(){
	std::cout << "Memoria cache de "
		  << mem_size << " B, "
		  << "mappeo directo, con bloques de "
		  << block_size << " bytes.\n\n\t"
		  << tag_size << " bits de tag, " << index_size << " bits de index." << std::endl
		  << "[Index]:\t Tag\t Estado" << std::endl;

	std::cout << std::hex;
	dir_t index;
	for(index = 0; index < set_vec.size(); index++){
		std::cout << index << "\t " << set_vec[index].m_tag << "\t " << set_vec[index].m_state << std::endl;
	}
	std::cout << std::dec << std::endl;

}

void CacheMemory::decode_dir(const dir_t &addr, dir_t &tag, dir_t &index){
	dir_t tag_and_index = addr >> offset_size;
	index = tag_and_index & my_mask;
	tag = tag_and_index >> index_size;
	return;
}

unsigned long CacheMemory::encode_dir(const dir_t &tag, const dir_t &index){
	dir_t addr = 0;
	addr = addr | tag;
	addr = addr << index_size;
	addr = addr | index;
	addr = addr << offset_size;
	return addr;
}

bool CacheMemory::read(const dir_t &addr){
	Block *line;
	dir_t tag, index, original;
	decode_dir(addr, tag, index);
	line = &set_vec[index];
	
	// Read Miss
	if(line->m_state == INVALID){
		read_miss(addr);
		line->m_tag = tag;
		line->m_state = SHARED;
		return false;
	}
	else{
		//Read Miss con substitucion
		if(line->m_tag != tag){
			// Write-back para datos modificados
			if(line->m_state == MODIFIED){
				original = encode_dir(line->m_tag, index);
				write_back(original);
			}
			read_miss(addr);
			line->m_tag = tag;
			line->m_state = SHARED;
			return false;
		}
		//Read Hit
		else{
			return true;
		}
	}
}

bool CacheMemory::write(const dir_t &addr){
	Block *line;
	dir_t tag, index, original;
	decode_dir(addr, tag, index);
	line = &set_vec[index];

	// Si hay un miss
	if(line->m_tag != tag || line->m_state == INVALID){
		if(line->m_state == MODIFIED){
			original = encode_dir(line->m_tag, index);
			write_back(original);
		}
		write_miss(addr);
		invalidate_broadcast(addr);
		line->m_tag = tag;
		line->m_state = MODIFIED;

		if(line->m_state > 3){
			std::cerr << "Estado inesperado en bloque " << std::hex << index << "," << tag << std::dec << std::endl;
		}
		return false;
	}
	else{	// Si hay un hit
		if(line->m_state != EXCLUSIVE && line->m_state != MODIFIED){
			invalidate_broadcast(addr);
		}
		line->m_state = MODIFIED;

		if(line->m_state > 3){
			std::cerr << "Estado inesperado en bloque " << std::hex << index << "," << tag << std::dec << std::endl;
		}

		return true;
	}

}

bool CacheMemory::snoop(const dir_t &addr){
	Block *line;
	dir_t tag, index;
	decode_dir(addr, tag, index);
	line = &set_vec[index];

	if(line->m_tag != tag || line->m_state == INVALID)
		return false;

	if(line->m_state == MODIFIED){
		write_back(addr);
		line->m_state = SHARED;
		return true;
	}
	if(line->m_state == EXCLUSIVE){
		line->m_state = SHARED;
		return true;
	}
	if(line->m_state > 3){
		std::cerr << "Estado inesperado en bloque " << std::hex << index << "," << tag << std::dec << std::endl;
	}
	return false;
}

bool CacheMemory::invalidate(const dir_t &addr){
	Block *line;
	dir_t tag, index;
	decode_dir(addr, tag, index);
	line = &set_vec[index];

	if(line->m_tag != tag || line->m_state == INVALID)
		return false;

	if(line->m_state == MODIFIED)
		write_back(addr);

	line->m_state = INVALID;
	return true;
}

int CacheMemory::read_miss(const dir_t &addr){
	if(bus)
		return bus->read_miss(addr, this);

	return -2;
}

int CacheMemory::write_miss(const dir_t &addr){
	if(bus)
		return bus->write_miss(addr, this);

	return -2;
}

void CacheMemory::write_back(const dir_t &addr){
	if(bus)
		bus->write_back(addr);

	return;
}

int CacheMemory::invalidate_broadcast(const dir_t &addr){
	if(bus)
		return bus->invalidate_broadcast(addr, this);
	return -2;
}


void CacheMemory::setBus(MemoryBus *const p){
	bus = p;
	bus->add_higher(this);
}
