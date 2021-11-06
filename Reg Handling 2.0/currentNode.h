#pragma once
#include "pch.h"

class currentNode
{
public:
	currentNode(); // Probably nothing?
	~currentNode();

	struct Value {
		char signature[3] = "00"; // 2 bytes
		unsigned short nameLength = 00; // 2 bytes
		unsigned short dataSize; // 4 bytes
		unsigned short dataOffset = 0000; // 4 bytes
		unsigned short dataType = 0000; // 4 bytes
		unsigned short flags = 00; // 2 bytes
		char name[60] = "00"; // name of value, CHECK MAX LENGTH, 60 INCORRECT
		std::vector<char> value; // actual value, string variables may not work for this purpose. Check during testing
	};

	struct Node { 
		int size = 0000;
		char signature[3] = "00"; // 2 bytes
		unsigned short flags = 00; // 2 bytes
		unsigned long long lastWriteTimestamp = 00000000; // 8 bytes
		unsigned int accessBits = 0000; // 4 bytes
		int parent = 0000;
		unsigned int subkeys[30]; // pointer to subkeys, CHECK MAX LENGTH, 30 INCORRECT
		int numValues = 0;
		Value values[30]; // array of values, CHECK MAX LENGTH, 30 INCORRECT
		unsigned short nameLength = 00;
		char name[60] = "00"; // name of node, CHECK MAX LENGTH, 60 INCORRECT
		int numSubkeys = 0000; // number of subkeys

	} current; // The data for the current node

	Node cuNode();
};

