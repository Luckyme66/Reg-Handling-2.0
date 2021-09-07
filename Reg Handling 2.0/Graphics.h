#pragma once
#include "File.h"
#include "currentNode.h"

class Graphics
{
public:
	Graphics(const char file[64]);
	~Graphics();

	void up();
	void down(int index);
	void display(); // Displays current cell info
	void displayFile(); // Displays some header info

private:
	File* file; // file opject to act on
	currentNode* manager; // current node object to act on
};

