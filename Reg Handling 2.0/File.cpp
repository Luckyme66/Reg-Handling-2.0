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
}

File::~File() {
	fs.close();
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