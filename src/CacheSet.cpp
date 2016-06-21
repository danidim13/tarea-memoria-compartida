#include "CacheSet.h"
#include <iostream>
#include <iomanip>

void Block::print(){
	std::cout << "tag: " << std::hex << m_tag << std::dec
		<< "\tstate: " << m_state << std::endl;
}
