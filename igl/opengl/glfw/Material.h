#pragma once
#include <vector>
#include "texture.h"
#include "shader.h"

class Material
{
	//Should use smart pointers for two materials with the same texture.
	unsigned int numOfTexs;
	std::vector<unsigned int> texIDs;
	std::vector<unsigned int> slotIDs;
	std::vector<unsigned int> shaderIndices;
	std::string name;
public:
	Material(unsigned int textureIDs[],unsigned int slots[],unsigned int size,std::string name = "defualt");
	 unsigned int GetTexture(unsigned int indx);
	 unsigned int GetSlot(unsigned int indx);
	 inline unsigned int GetNumOfTexs() { return numOfTexs; }
	 void Bind(const std::vector<Texture*> textures, int indx);
	 void rotateTexs();
	 std::string getName() {
		 return name;
	 }

	~Material();
};

