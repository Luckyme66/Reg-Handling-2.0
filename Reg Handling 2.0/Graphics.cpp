#include "Graphics.h"

Graphics::Graphics(const char fileName[64]) {
	file = new File(fileName);
}

Graphics::~Graphics() {
	file->closeFile();
}

void Graphics::start() {
	updateGlobalCurrent();

	display();
	std::cout << "\n";
	displayFile();
	
	file->down(0);
	updateGlobalCurrent();
	std::cout << "\n";
	display();

	file->down(0);
	updateGlobalCurrent();
	std::cout << "\n";
	display();
}

void Graphics::display() {
	std::cout << "Size: " << globalCurrent.current.size << "\n";
	std::cout << "Signature: " << globalCurrent.current.signature << "\n";
	std::cout << "Flags: " << globalCurrent.current.flags << "\n";
	std::cout << "Last Write Timestamp: " << globalCurrent.current.lastWriteTimestamp << "\n";
	std::cout << "Access Bits: " << globalCurrent.current.accessBits << "\n";
	std::cout << "Parent: " << globalCurrent.current.parent << "\n";
	std::cout << "Name: " << globalCurrent.current.name << "\n";
}

void Graphics::displayFile() {
	std::cout << "Signature: " << file->signature << "\n";
	std::cout << "Last Write Timestamp: " << file->lastWriteTimestamp << "\n";
	std::cout << "Major Verion Number: " << file->majorVersionNumber << "\n";
	std::cout << "Minor Version Number: " << file->minorVersionNumber << "\n";
	std::cout << "Root Cell Offset: " << file->rootCellOffset << "\n";
	std::cout << "Length: " << file->length << "\n";
	std::cout << "Name: " << file->fileName << "\n";
}

void Graphics::updateGlobalCurrent() {
	globalCurrent.current = file->current();
}