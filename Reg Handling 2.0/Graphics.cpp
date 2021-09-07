#include "Graphics.h"

Graphics::Graphics(const char fileName[64]) {
	file = new File(fileName);
	manager = new currentNode();
	manager->update(file->rootCellOffset, file);
}

Graphics::~Graphics() {
	delete file;
	delete manager;
}

void Graphics::up() {
	// Change node stored by manager to parent of current node
	manager->update(manager->current.parent, file);
}

void Graphics::down(int index) {
	// Change node stored by manager to subkey of current node at index passed
	manager->update(manager->current.subkeys[index], file);
}

void Graphics::display() {
	std::cout << "Signature: " << manager->current.signature << "\n";
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