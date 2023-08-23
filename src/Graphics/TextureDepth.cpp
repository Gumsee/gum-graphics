#include "TextureDepth.h"

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