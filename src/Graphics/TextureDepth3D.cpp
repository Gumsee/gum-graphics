#include "TextureDepth3D.h"

TextureDepth3D::TextureDepth3D(const unsigned int& numlayers, std::string name, uint16_t datatype)
    : Texture(name, TEXTUREDEPTH3D)
{
	this->sName = name;
	this->iNumLayers = numlayers;
    this->iDatatype = datatype;
    createNative();
    setGrayscale(true);
    setTransparency(false);
}

TextureDepth3D::~TextureDepth3D()
{
}

//
// Setter
//
void TextureDepth3D::setSize(const ivec2& size) { this->v2Size = size; updateImage(); }


//
// Getter
//
ivec2 TextureDepth3D::getSize() const			{ return this->v2Size; }
unsigned int TextureDepth3D::numLayers() const  { return this->iNumLayers; }