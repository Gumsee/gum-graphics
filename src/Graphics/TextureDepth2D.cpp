#include "TextureDepth2D.h"

TextureDepth2D::TextureDepth2D(std::string name, uint16_t datatype)
    : Texture(name, TEXTUREDEPTH2D)
{
	this->sName = name;
    this->iDatatype = datatype;
    createNative();
    setGrayscale(true);
    setTransparency(false);
}

TextureDepth2D::~TextureDepth2D()
{
}

//
// Setter
//
void TextureDepth2D::setSize(const ivec2& size) { this->v2Size = size; updateImage(); }


//
// Getter
//
ivec2 TextureDepth2D::getSize() const			  { return this->v2Size; }