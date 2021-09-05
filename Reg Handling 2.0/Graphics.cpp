#include "Graphics.h"

Graphics::Graphics(File* filee, currentNode* managerr)
{
	file = filee;
	manager = managerr;
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
	std::cout << manager->current.name;

}