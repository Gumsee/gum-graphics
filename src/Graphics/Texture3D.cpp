#include "Texture3D.h"
#include <GL/glew.h>
#include <cstdint>
#include <iostream>
#include <future>

Texture3D::Texture3D(std::string name, uint16_t datatype)
    : Texture(TEXTURE3D, datatype)
{
	this->sName = name;
}

void Texture3D::load(std::string TexFilepath, bool wait)
{ 
}

//
// Setter
//
void Texture3D::setSize(ivec3 size)                       { this->v3Size = size; updateImage(); }
void Texture3D::setData(std::vector<unsigned char> data)  { this->vPixelData = data; }
void Texture3D::setPixel(int x, int y, vec4 color) 
{ 
    int pos = v3Size.x * y + x;
    vPixelData[pos + 0] = color.x;
    vPixelData[pos + 1] = color.y;
    vPixelData[pos + 2] = color.z;
    vPixelData[pos + 3] = color.w; 
}


//
// Getter
//
ivec3 Texture3D::getSize() 						        { return this->v3Size; }
const unsigned char* Texture3D::getPixelPtr() 	        { return &this->vPixelData[0]; }
vec4 Texture3D::getPixel(int x, int y) 	        
{ 
    int pos = v3Size.x * y + x;
    return vec4(vPixelData[pos + 0], vPixelData[pos + 1], vPixelData[pos + 2], vPixelData[pos + 3]);
}