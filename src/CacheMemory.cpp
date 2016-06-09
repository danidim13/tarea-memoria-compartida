#include "CacheMemory.h"
#include "CacheSet.h"
#include <sstream>
#include <cmath>
#include <iostream>

CacheMemory::CacheMemory(){
	block_size = 0;
        assoc = 0;
        mem_size = 0;

        tag_size = 0;
        index_size = 0;
        offset_size = 0;
        block_num = 0;
        set_num = 0;
}

CacheMemory::CacheMemory(const dir_t &v_assoc, const dir_t &v_mem_size, const dir_t &v_block_size){
	//df
	set_assoc(v_assoc);
	set_mem_size(v_mem_size);
	set_block_size(v_block_size);
	initialize();
}

CacheMemory::~CacheMemory(){
}

void CacheMemory::initialize(){
	// Cantidad de bloques y sets
	block_num = mem_size/block_size;
	set_num = block_num/assoc;

	// Se determinan los bits del offset
	offset_size = (int) log2(block_size);
	index_size = (int) log2(set_num);
	tag_size = DIR_SIZE - (offset_size + index_size);

	set_vec.resize(set_num, CacheSet(assoc));

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
	if (check_pow2(val))
		mem_size = val;
	else{
		std::cerr << "Error, mem_size not valid" << std::endl;
		mem_size = 1;
	}
}

void CacheMemory::set_assoc(const dir_t &val){
	if (check_pow2(val))
		assoc = val;
	else{
		std::cerr << "Error, assoc not valid" << std::endl;
		assoc = 1;
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
		  << assoc << "-way associative, con bloques de "
		  << block_size << " bytes.\n\n\t"
		  << tag_size << " bits de tag, " << index_size << " bits de index." << std::endl;
		}

bool CacheMemory::fetch(const dir_t &addr){
  tag_and_index = addr >> offset_size;
	index = tag_and_index & my_mask;
	tag = tag_and_index >> index_size;
  bool hit;
  hit = set_vec[index].fetch(tag);
  return hit;
}
