#include "currentNode.h"

currentNode::currentNode() {

}

void currentNode::update(unsigned int address, File* file) {
	// address - offset relative to start of hbin records of new node
	// file - file object containing node
	file->getChars(4096 + address, 2, (char*)&current.signature);
	file->readNums(4096 + address + 2, 2, (char*)&current.flags);
	file->readNums(4096 + address + 4, 8, (char*)&current.lastWriteTimestamp);
	file->readNums(4096 + address + 12, 4, (char*)&current.accessBits);
	file->readNums(4096 + address + 16, 4, (char*)&current.parent);

	// Harvest subkeys
	int numSubkeys;
	file->readNums(4096 + address + 20, 4, (char*)&numSubkeys);

	unsigned int subKeyLocation;
	file->readNums(4096 + address + 28, 4, (char*)&subKeyLocation);

	subKeyHarvest(subKeyLocation, file);

	// Harvest values
	int numValues;
	file->readNums(4096 + address + 36, 4, (char*)&numValues);

	unsigned int valueLocation;
	file->readNums(4096 + address + 40, 4, (char*)&valueLocation);

	valueHarvest(valueLocation, numValues, file);

	// Harvest name
	short nameLength;
	file->readNums(4096 + address + 72, 2, (char*)&nameLength);

	file->readNums(4096 + address + 76, nameLength, (char*)&current.name);
}

void currentNode::subKeyHarvest(unsigned int subKeyLocation, File* file) {
	char sig[2]; // signature
	file->getChars(4096 + subKeyLocation, 2, (char*)&sig);

	int elms; // num elements in structure
	file->readNums(4096 + subKeyLocation + 2, 2, (char*)elms);

	int tempOffset = 4; // Used to track offset relative to start of index, 4 bytes for signature and num elms

	switch (int(sig)) {

	case(int("li")):
		for (int i = 0; i < elms; i++) {
			file->readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&current.subkeys[i]);
			tempOffset += 4;
		}

	case(int("lf")):
		for (int i = 0; i < elms; i++) {
			file->readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&current.subkeys[i]);
			tempOffset += 8;
		}

	case(int("lh")):
		for (int i = 0; i < elms; i++) {
			file->readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&current.subkeys[i]);
			tempOffset += 8;
		}

	case(int("ri")):
		for (int i = 0; i < elms; i++) {
			unsigned int tempLocation; // stores location of subkey list
			file->readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&tempLocation);
			subKeyHarvest(tempLocation, file);
			tempOffset += 4;
		}
	}
}

void currentNode::valueHarvest(unsigned int valueLocation, int numValues, File* file) {
	int tempOffset = 0; // Used to track offset relative to start of list

	for (int i = 0; i < numValues; i++) {
		unsigned int offset; // offset of key value structure
		file->readNums(4096 + valueLocation + tempOffset, 4, (char*)&offset);

		file->readNums(4096 + offset, 2, (char*)&current.values[i].signature);
		file->readNums(4096 + offset + 2, 2, (char*)&current.values[i].nameLength);
		file->readNums(4096 + offset + 4, 4, (char*)&current.values[i].dataSize);
		file->readNums(4096 + offset + 8, 4, (char*)&current.values[i].dataOffset);

		if (current.values[i].dataSize & (1<<31)){ // Check if msb is 1
			file->readNums(4096 + offset + 8, (current.values[i].dataSize &= ~(1UL<<31)), (char*)&current.values[i].value);
		}
		else {
			char sig[2];
			file->getChars(4096 + current.values[i].dataOffset, 2, (char*)&sig);
			if (sig == "db"){
				int numSeg;
				file->readNums(4096 + current.values[i].dataOffset + 2, 2, (char*)&numSeg);

				unsigned int tempLocation;
				file->readNums(4096 + current.values[i].dataOffset + 4, 4, (char*)&tempLocation);

				int tempOffset2 = 0;
				for (int x = 0; x < numSeg; x++) {
					unsigned int dataLocation;
					file->readNums(4096 + tempLocation + tempOffset2, 4, (char*)&dataLocation);

					int size;
					if (current.values[i].dataSize - (16344*x) > 16344){
						size = 16344;
					}
					else {
						size = current.values[i].dataSize - (16344 * x);
					}

					std::string temp; //string variables may not work for this purpose. Check during testing
					file->readNums(4096 + dataLocation + 4, size, (char*)&temp);

					current.values[i].value += temp;

					tempOffset += 4;
				}
			}
			else {
				file->readNums(4096 + current.values[i].dataOffset, (current.values[i].dataSize &= ~(1UL << 31)), (char*)&current.values[i].value);
			}
		}
		file->readNums(4096 + offset + 12, 4, (char*)&current.values[i].dataType);
		file->readNums(4096 + offset + 16, 2, (char*)&current.values[i].flags);
		file->getChars(4096 + offset + 20, current.values[i].nameLength, (char*)&current.values[i].name);

		tempOffset += 4;
	}
}