#include "TextureDepth.h"
#include <GL/glew.h>
#include <iostream>
#include <future>
#include "WrapperFunctions.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <Codecs/TextureLoader.h>
#include <Essentials/Tools.h>
#include <vector>

TextureDepth::TextureDepth(std::string name, uint16_t datatype)
    : Texture(TEXTUREDEPTH, datatype)
{
	this->sName = name;
    createNative();
}

TextureDepth::~TextureDepth()
{
}

//
// Setter
//
void TextureDepth::setSize(const ivec2& size) { this->v2Size = size; updateImage(); }


//
// Getter
//
ivec2 TextureDepth::getSize() const			  { return this->v2Size; }