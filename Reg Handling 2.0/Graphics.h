#pragma once
#include "File.h"
#include "currentNode.h"

class Graphics
{
public:
	Graphics(File* filee, currentNode* managerr);
	void up();
	void down(int index);
	void display();

private:
	File* file; // file opject to act on
	currentNode* manager; // current node object to act on
};

