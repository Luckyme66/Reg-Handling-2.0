#pragma once
#include "File.h"
#include "currentNode.h"

class Graphics
{
public:
	Graphics(const char file[64]);
	~Graphics();

	void updateGlobalCurrent();

	void display(); // Displays current cell info
	void displayFile(); // Displays some header info

	void start();

private:
	File* file; // file opject to act on
	currentNode globalCurrent;
};