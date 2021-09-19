#pragma once
#include "pch.h"

class File
{
public:
	File(const char file[64]); // Harvests header
	~File();

	void getChars(unsigned int offset, int length, char* ptr); // Read characters and terminate with null
	void readNums(unsigned int offset, int length, char* ptr); // Read characters raw

	unsigned int offset = 4096; // offset of hbin start

	char signature[64]; //signature
	long long lastWriteTimestamp; //64-bit int of last write timestamp as a windows FILETIME
	int majorVersionNumber; //32-bit int of major version number stored in little endian
	int minorVersionNumber; //32-bit int of minor version number stored in little endian
	unsigned int rootCellOffset; //32-bit unsigned int of offset of root cell
	unsigned int length; //32-bit unsigned int of the total size of all currently in-use hbin cells
	char fileName[64]; //printable file name (possibly UTF-16 little-endian of file name)

private:
	std::ifstream fs; //file stream containing the hive
};

