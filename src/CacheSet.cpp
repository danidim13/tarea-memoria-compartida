#include "CacheSet.h"
#include <iostream>
//#define DEBUG 1

CacheSet::CacheSet(): blocks(0), fifo(0), block_num(0){
	#ifdef DEBUG
	std::cout << "Constructor por defecto!" << std::endl;
	#endif

}

CacheSet::CacheSet(int assoc):fifo(0), block_num(assoc){
	blocks = new Block[assoc];

	#ifdef DEBUG
	std::cout << "Constructor por associa!" << std::endl;
	#endif
}

CacheSet::CacheSet(const CacheSet& other):blocks(0), fifo(other.fifo), block_num(other.block_num){
	if(block_num){
		blocks = new Block[block_num];
		for(int i=0; i<block_num; i++){
			blocks[i] = other.blocks[i];
		}
	}

	#ifdef DEBUG
	std::cout << "Constructor por copia!" << std::endl;
	#endif
}

CacheSet::~CacheSet(){
	delete[] blocks;
	#ifdef DEBUG
	std::cout << "Destructor por defecto!" << std::endl;
	#endif
}

CacheSet& CacheSet::operator=(const CacheSet& rhs){
	delete[] blocks;
	blocks = 0;
	block_num = rhs.block_num;
	fifo = rhs.block_num;
	if(block_num){
		blocks = new Block[block_num];
		for(int i=0; i<block_num; i++){
			blocks[i] = rhs.blocks[i];
		}
	}

	#ifdef DEBUG
	std::cout << "Assign operator!" << std::endl;
	#endif
	return *this;
}

Block* CacheSet::find(dir_t tag){
	for(int i=0; i < block_num; i++){
		if(blocks[i].m_valid && blocks[i].m_tag == tag)
			return blocks + i;
	}
	return 0;
}

bool CacheSet::fetch(dir_t tag){
	Block *p = find(tag);
	if(p){
		return true;
	}
	else{
		blocks[fifo].m_valid = true;
		blocks[fifo].m_tag = tag;
		fifo = (fifo + 1)%block_num;
		return false;
	}
}

void CacheSet::print(){
	std::cout << "\n";
	if(!blocks)
		std::cout << "Set vacio!" << std::endl;

	for(int i=0; i < block_num; i++){
		std::cout << i << ": ";
		if(blocks[i].m_valid){
			std::cout << "valid, tag: " << blocks[i].m_tag;
		}
		else{
			std::cout << "invalid";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;

}
