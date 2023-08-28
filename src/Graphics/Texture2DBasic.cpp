#include "Texture2DBasic.h"

Texture2DBasic::Texture2DBasic()
{
    createNative();
}

Texture2DBasic::~Texture2DBasic()
{
    destroyNative();
}


//
// Setter
//
void Texture2DBasic::setData(const ivec2& size, const int& numchannels, const void* data, uint16_t datatype)
{ 
    v2Size = size;
    iChannels = numchannels;
    updateImage(size, numchannels, data, datatype);
}


//
// Getter
//
ivec2 Texture2DBasic::getSize() const   { return this->v2Size; }
int Texture2DBasic::numChannels() const { return this->iChannels; }