#include "Texture2D.h"
#include <iostream>
#include <future>
#include "WrapperFunctions.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <Codecs/TextureLoader.h>
#include <Essentials/Tools.h>
#include <vector>

Texture2D::Texture2D(std::string name, uint16_t datatype)
    : Texture(TEXTURE2D, datatype)
{
	this->sName = name;
	this->iChannels = 4;
    this->vPixelData = nullptr;
    repeat();
    setFiltering(FilteringTypes::LINEAR);
}

Texture2D::~Texture2D()
{
    if(bNeedsFreeing && vPixelData != nullptr)
        free(vPixelData);
}

float Texture2D::getHeightMapPixel(int x, int y)
{
	if(x < 0 || x > v2Size.x || 
       y < 0 || y > v2Size.y)
	{
		return 0.0f;
	}
	else
	{
		float returnHeight = getPixel(x, y).x * getPixel(x, y).y * getPixel(x, y).z;
		returnHeight += MAX_PIXEL_COLOR / 2.0f;
		returnHeight /= MAX_PIXEL_COLOR / 2.0f;
		return returnHeight;
	}
}

void Texture2D::load(std::string TexFilepath, bool wait)
{ 
    std::thread loadThread([TexFilepath, this] {
        std::lock_guard<decltype(loadMutex)> lock(loadMutex);

        ImageData imageData = TextureLoader::loadImage(TexFilepath);
        v2Size = ivec2(imageData.width, imageData.height);
        iChannels = imageData.numComps;
        vPixelData = imageData.data;
        bNeedsFreeing = true;
        bIsGrayscale = (iChannels <= 2);

        vToLoadTextures.push_back(this);
        markLoaded();
    });

    if(wait)
    {
        loadThread.join();
        updateImage();
    }
    else
    {
        loadThread.detach();
    }
}

void Texture2D::loadFromMemory(unsigned char* pixels, size_t size)
{
    ImageData imageData = TextureLoader::loadImage(pixels, size);
    iChannels = imageData.numComps;
    bIsGrayscale = (iChannels <= 2);
    v2Size = ivec2(imageData.width, imageData.height);
    vPixelData = imageData.data;
    bNeedsFreeing = true;

    updateImage();
}
    
void Texture2D::initEmpty()
{
    size_t dataSize = v2Size.x * v2Size.y * iChannels;
    vPixelData = (unsigned char*)malloc(dataSize);
    memset(vPixelData, 0, dataSize);
}


//
// Setter
//
void Texture2D::setData(void* data)  { this->vPixelData = data; updateImage(); }
void Texture2D::setSize(const ivec2& size)
{ 
    /*size_t oldDataSize = v2Size.x * v2Size.y * iChannels;
    size_t newDataSize = size.x * size.y * iChannels;

    vPixelData = realloc(vPixelData, newDataSize);

    if(oldDataSize < newDataSize) // Add data
        memset(vPixelData, oldDataSize, newDataSize - oldDataSize);*/

    this->v2Size = size; 
    updateImage(); 
}

void Texture2D::setPixel(const int& x, const int& y, const color& col) 
{
    unsigned int pos = v2Size.x * y * iChannels + x * iChannels;
    if(pos >= v2Size.x * v2Size.y * iChannels)
        return;
    for(unsigned int i = 0; i < iChannels; i++)
        ((unsigned char*)vPixelData)[pos + i] = col[i];
}
void Texture2D::setNumChannels(const int& channels)
{
    if(channels > 4)
    {
        Gum::Output::error("Invalid number of channels");
        return;
    }
    this->iChannels = channels;
}


//
// Getter
//
ivec2 Texture2D::getSize() const				{ return this->v2Size; }
const void* Texture2D::getPixelPtr() 	        { return this->vPixelData; }
vec4 Texture2D::getPixel(int x, int y) const
{ 
    int pos = v2Size.x * y * iChannels + x * iChannels;
    vec4 retcol;
    for(unsigned int i = 0; i < iChannels; i++)
        retcol[i] = (float)((unsigned char*)vPixelData)[pos + i] / 255.0f;
    return retcol;
}
int Texture2D::numChannels() const                      { return this->iChannels; }