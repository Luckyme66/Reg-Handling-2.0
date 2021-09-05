#include "currentNode.h"

currentNode::currentNode() {

}

void currentNode::update(int address, File* file) {
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

	int subKeyLocation;
	file->readNums(4096 + address + 28, 4, (char*)&subKeyLocation);

	subKeyHarvest(subKeyLocation, file);

	// Harvest values
	int numValues;
	file->readNums(4096 + address + 36, 4, (char*)&numValues);

	int valueLocation;
	file->readNums(4096 + address + 40, 4, (char*)&valueLocation);

	valueHarvest(valueLocation, numValues, file);
}

void currentNode::subKeyHarvest(int subKeyLocation, File* file) {
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
			int tempLocation; // stores location of subkey list
			file->readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&tempLocation);
			subKeyHarvest(tempLocation, file);
			tempOffset += 4;
		}
	}
}

void currentNode::valueHarvest(int valueLocation, int numValues, File* file) {
	int tempOffset = 0; // Used to track offset relative to start of list

	for (int i = 0; i < numValues; i++) {
		int offset; // offset of key value structure
		file->readNums(4096 + valueLocation + tempOffset, 4, (char*)&offset);

		file->readNums(4096 + offset, 4, (char*)&offset);
		tempOffset += 4;
	}
}