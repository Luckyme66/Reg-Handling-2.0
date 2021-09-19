#include "pch.h"
#include "currentNode.h"
#include "File.h"
#include "Graphics.h"

int main(){
	// Graphics handles user input + co-ordinates procesing
	// 	   Stores one current node and one file object to ensure data about both kept available
	// currentNode stores info on the node currently in access and methods for updating this data
	// File stores the file stream and methods for interacting with it, alongside file-wide metadata 
	Graphics screen("C:\\Users\\lucy\\Documents\\SANS Donald Blake\\config\\SAM");

	screen.display();
	screen.displayFile();

	screen.down(0);
	screen.display();
}