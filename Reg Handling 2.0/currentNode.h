#pragma once
#include "pch.h"
#include "File.h"

class currentNode
{
public:
	currentNode(); // Probably nothing?
	~currentNode(); // Also likely unused (turn into a singleton?)

	struct Value {
		char signature[2]; // 2 bytes
		unsigned short nameLength; // 2 bytes
		unsigned short dataSize; // 4 bytes
		unsigned short dataOffset; // 4 bytes
		unsigned short dataType; // 4 bytes
		unsigned short flags; // 2 bytes
		char name[60]; // name of value, CHECK MAX LENGTH, 60 INCORRECT
		char value[60]; // actual value, CHECK MAX LENGTH, 60 INCORRECT
	};

	struct Node { 
		char signature[2]; // 2 bytes
		unsigned short flags; // 2 bytes
		int lastWriteTimestamp; // 8 bytes
		short accessBits; // 4 bytes
		unsigned int parent;
		unsigned int subkeys[30]; // pointer to subkeys, CHECK MAX LENGTH, 30 INCORRECT
		Value values[30]; // array of values, CHECK MAX LENGTH, 30 INCORRECT
		char name[60]; // name of node, CHECK MAX LENGTH, 60 INCORRECT

	} current; // The data for the current node

	void update(unsigned int address, File *file); // Harvests the data for the current node

private:
	void subKeyHarvest(unsigned int subKeyLocation, File* file); // Specifically handles subkey harvesting
	void valueHarvest(unsigned int valueLocation, int numValues, File* file); // Specifically handles value harvesting
};

