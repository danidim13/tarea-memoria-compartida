#ifndef CACHESET_H
#define CACHESET_H
#include <iostream>

#define INVALID 0
#define SHARED 1
#define EXCLUSIVE 2
#define MODIFIED 3

struct Block{
	Block():m_state(INVALID),m_tag(0){};
	~Block(){};
	int m_state;
	unsigned long m_tag;

	void print();
};

#endif
