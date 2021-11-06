#include "File.h"

void utf8_ut16(char* source, int sizeOfSource, char* dest) {
	char* data = new char[sizeOfSource / 2];

	for (int i = 0; i < sizeOfSource; i = i + 2) {
		char val = *(source + i);
		if (val == 0x00) {
			data[i / 2] = val;
			break;
		}
		else {
			data[i / 2] = val;
		}
	}

	strcpy_s(dest, 64, data);
	delete[] data;
}

File::File(const char file[64]) {
	fs.open(file, std::ifstream::in | std::ifstream::binary); //Open file

	//HARVEST HEADER

	//Get file signature
	getChars(0x0, 4, (char*)&signature);

	//Get last write timestamp
	readNums(0xc, 8, (char*)&lastWriteTimestamp);

	//Get Major version number
	readNums(0x14, 4, (char*)&majorVersionNumber); //NOTE: This appears to only be correct for 2 bytes, though Eric suggests it should be 4

	//Get Minor version number
	readNums(0x18, 4, (char*)&minorVersionNumber); //NOTE: This appears to only be correct for 2 bytes, though Eric suggests it should be 4

	//Get Root cell offset
	readNums(0x24, 4, (char*)&rootCellOffset);

	//Get file length
	readNums(0x28, 4, (char*)&length);

	//Get file name and switch to different utf so printable
	char temp[64];
	getChars(0x30, 63, (char*)&temp);
	utf8_ut16(temp, 64, fileName);

	update(rootCellOffset);
}

void File::closeFile() {
	fs.close();
}

File::~File() {
	
}

void File::up() {
	update(cN.current.parent);
}

void File::down(int index) {
	int val = cN.current.subkeys[index];
	update(val);
}

currentNode::Node File::current() {
	currentNode::Node Test = cN.cuNode();
	return (Test);
}


void File::update(unsigned int address){
	// address - offset relative to start of hbin records of new node
	// file - file object containing node

	readNums(4096 + address, 4, (char*)&cN.current.size);

	address += 4;

	getChars(4096 + address, 2, (char*)&cN.current.signature);
	readNums(4096 + address + 2, 2, (char*)&cN.current.flags);
	readNums(4096 + address + 4, 8, (char*)&cN.current.lastWriteTimestamp);
	readNums(4096 + address + 12, 4, (char*)&cN.current.accessBits);
	readNums(4096 + address + 16, 4, (char*)&cN.current.parent);

	// Harvest subkeys
	readNums(4096 + address + 20, 4, (char*)&cN.current.numSubkeys);

	unsigned int subKeyLocation;
	readNums(4096 + address + 28, 4, (char*)&subKeyLocation);

	subKeyHarvest(subKeyLocation);

	// Harvest values
	readNums(4096 + address + 36, 4, (char*)&cN.current.numValues);

	unsigned int valueLocation;
	readNums(4096 + address + 40, 4, (char*)&valueLocation);

	//valueHarvest(valueLocation, cN.current.numValues);

	// Harvest name
	readNums(4096 + address + 72, 2, (char*)&cN.current.nameLength);

	getChars(4096 + address + 76, cN.current.nameLength, (char*)&cN.current.name);
}

void File::subKeyHarvest(unsigned int subKeyLocation) {
	subKeyLocation += 4;
	short sig; // signature
	readNums(4096 + subKeyLocation, 2, (char*)&sig);

	unsigned short elms; // num elements in structure
	readNums(4096 + subKeyLocation + 2, 2, (char*)&elms);

	int tempOffset = 4; // Used to track offset relative to start of index, 4 bytes for signature and num elms

	switch (sig) {

	case(0x696c): // li
		for (int i = 0; i < elms; i++) {
			readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&cN.current.subkeys[i]);
			tempOffset += 4;
		}
		break;

	case(0x666c): // lf
		for (int i = 0; i < elms; i++) {
			readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&cN.current.subkeys[i]);
			tempOffset += 8;
		}
		break;

	case(0x686c): // lh
		for (int i = 0; i < elms; i++) {
			readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&cN.current.subkeys[i]);
			tempOffset += 8;
		}
		break;

	case(0x6972): // ri
		for (int i = 0; i < elms; i++) {
			unsigned int tempLocation; // stores location of subkey list
			readNums(4096 + subKeyLocation + tempOffset, 4, (char*)&tempLocation);
			subKeyHarvest(tempLocation);
			tempOffset += 4;
		}
		break;
	}
}

void File::valueHarvest(unsigned int valueLocation, int numValues) {
	int tempOffset = 4; // Used to track offset relative to start of list

	for (int i = 0; i < numValues; i++) {
		unsigned int offset; // offset of key value structure
		readNums(4096 + valueLocation + tempOffset, 4, (char*)&offset);

		offset += 4;
		getChars(4096 + offset, 2, (char*)&cN.current.values[i].signature);
		readNums(4096 + offset + 2, 2, (char*)&cN.current.values[i].nameLength);
		readNums(4096 + offset + 4, 4, (char*)&cN.current.values[i].dataSize);


		cN.current.values[i].value.reserve(cN.current.values[i].dataSize);

		readNums(4096 + offset + 8, 4, (char*)&cN.current.values[i].dataOffset);

		if (cN.current.values[i].dataSize & (1 << 31)) { // Check if msb is 1 // WARNING THIS DOES NOT WORK. PLS FIX. CAUSE OF ERROR.
			readNums(4096 + offset + 8, (cN.current.values[i].dataSize &= ~(1UL << 31)), (char*)&cN.current.values[i].value);
			std::cout << "yaha!\n";
		}
		else {
			char sig[3];
			getChars(4096 + cN.current.values[i].dataOffset + 4, 2, (char*)&sig);
			if (sig == "db") {
				int numSeg;
				readNums(4096 + cN.current.values[i].dataOffset + 2, 2, (char*)&numSeg);

				unsigned int tempLocation;
				readNums(4096 + cN.current.values[i].dataOffset + 4, 4, (char*)&tempLocation);

				int tempOffset2 = 0;
				for (int x = 0; x < numSeg; x++) {
					unsigned int dataLocation;
					readNums(4096 + tempLocation + tempOffset2, 4, (char*)&dataLocation);

					int size;
					if (cN.current.values[i].dataSize - (16344 * x) > 16344) {
						size = 16344;
					}
					else {
						size = cN.current.values[i].dataSize - (16344 * x);
					}

					char *temp = new char[size];
					readNums(4096 + dataLocation + 4, size, (char*)&temp);
					cN.current.values[i].value[cN.current.values[i].dataSize - (16344 * x)] = *temp;
					delete temp;

					tempOffset += 4;
				}
			}
			else {
				readNums(4096 + cN.current.values[i].dataOffset + 4, (cN.current.values[i].dataSize &= ~(1UL << 31)), (char*)&cN.current.values[i].value);
			}
		}

		readNums(4096 + offset + 12, 4, (char*)&cN.current.values[i].dataType);
		readNums(4096 + offset + 16, 2, (char*)&cN.current.values[i].flags);
		getChars(4096 + offset + 20, cN.current.values[i].nameLength, (char*)&cN.current.values[i].name);

		tempOffset += 4;
	}
}

void File::getChars(unsigned int offset, int length, char* ptr) {
	//offset - offset relative to file start
	//length - number of characters to read
	//ptr - pointer to location to store output
	//function reads 'length' number of chars from 'offset' and writes it to location ptr

	int truLength = length + 1; //Account for terminating character
	(fs).seekg(offset); //Move to correct place in file
	(fs).get(ptr, truLength); //Store 'truLength' characters from file to desired location, with an a null terminator as the last char
}

void File::readNums(unsigned int offset, int length, char* ptr) {
	//offset - offset relative to file start
	//length - number of characters to read
	//ptr - pointer to location to store output
	//function reads 'length' number of chars from 'offset' and writes it to location ptr

	(&fs)->seekg(offset); //Move to correct place in file
	(&fs)->read(ptr, length); //Store 'length' characters from file to desired location
}