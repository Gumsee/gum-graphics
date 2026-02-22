#include "Texture2D.h"
#include "Codecs/Filetypes.h"
#include "Codecs/ImageData.h"
#include "Texture.h"
#include "TextureImageData.h"
#include "Variables.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <Codecs/TextureLoader.h>
#include <Essentials/Tools.h>
#include <thread>

template<typename T>
tTexture2D<T>::tTexture2D(std::string name)
    : Texture(name, TEXTURE2D)
{
    if(!Gum::Graphics::RUN_HEADLESS)
    {
        repeat();
        setFiltering(LINEAR);
    }
}

template<typename T>
tTexture2D<T>::tTexture2D(const std::string& name, const ivec2& size, Texture2DGeneratorFunc generator)
    : tTexture2D(name)
{
    v2Size = size;
    initEmpty();
    this->generate(generator);
}

template<typename T>
tTexture2D<T>::~tTexture2D()
{
}

template<typename T>
float tTexture2D<T>::getHeightMapPixel(int x, int y)
{
    color pixel = getPixel(x, y);
    float returnHeight = pixel.r * pixel.g * pixel.b;
    returnHeight += MAX_PIXEL_COLOR / 2.0f;
    returnHeight /= MAX_PIXEL_COLOR / 2.0f;
    return returnHeight;
}

template<typename T>
void tTexture2D<T>::load(const Gum::File& filepath, bool wait)
{ 
    std::thread loadThread([filepath, this] {
        std::lock_guard<decltype(this->loadMutex)> lock(this->loadMutex);

        ImageData imageData = TextureLoader::loadImage(filepath);
        v2Size = ivec2(imageData.width, imageData.height);
        this->setNumChannels(imageData.numComps, 0);
        this->setDataPtr((T*)imageData.data, 0);
        this->setNeedsFreeing(true, 0);
        this->bIsGrayscale = (this->numChannels(0) <= 2);

        this->vToLoadTextures.push_back(this);
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

template<typename T>
void tTexture2D<T>::loadFromMemory(unsigned char* pixels, size_t size)
{
    ImageData imageData = TextureLoader::loadImage(pixels, size);
    this->setNumChannels(imageData.numComps, 0);
    bIsGrayscale = (this->numChannels(0) <= 2);
    v2Size = ivec2(imageData.width, imageData.height);
    this->setDataPtr((T*)imageData.data, 0);
    //bNeedsFreeing = true;

    updateImage();
}

template<typename T>
void tTexture2D<T>::generate(std::function<color(ivec2 pixelcoord)> function)
{
    for(int x = 0; x < v2Size.x; x++)
    {
        for(int y = 0; y < v2Size.y; y++)
        {
            color col = function(ivec2(x,y));
            unsigned int pos = v2Size.x * y * this->numChannels(0) + x * this->numChannels(0);
            for(unsigned int i = 0; i < this->numChannels(0); i++)
                this->setDataAt(pos + i, 0, col[i]);
        }
    }
    updateImage();
}
    
template<typename T>
void tTexture2D<T>::initEmpty()
{
    size_t dataSize = v2Size.x * v2Size.y * this->numChannels(0) * sizeof(T);
    this->setDataPtr((T*)malloc(dataSize), 0);
    memset(this->getDataPtr(0), 0, dataSize);
}


//
// Setter
//
template<typename T> void tTexture2D<T>::setData(T* data)  { this->setDataPtr(data, 0); updateImage(); }
template<typename T> void tTexture2D<T>::setSize(const ivec2& size)
{ 
    /*size_t oldDataSize = v2Size.x * v2Size.y * iChannels;
    size_t newDataSize = size.x * size.y * iChannels;

    vPixelData = realloc(vPixelData, newDataSize);

    if(oldDataSize < newDataSize) // Add data
        memset(vPixelData, oldDataSize, newDataSize - oldDataSize);*/

    this->v2Size = size; 
    updateImage(); 
}

template<typename T>
void tTexture2D<T>::setPixel(const int& x, const int& y, const color& col) 
{
    unsigned int pos = v2Size.x * y * this->numChannels(0) + x * this->numChannels(0);
    if(pos >= v2Size.x * v2Size.y * this->numChannels(0))
        return;
    for(unsigned int i = 0; i < this->numChannels(0); i++)
        this->setDataAt(pos + i, 0, col[i]);
}

template<typename T>
void tTexture2D<T>::setSerializeQuality(const unsigned int& quality)
{
    iSerializeQuality = quality;
}


//
// Getter
//
template<typename T> ivec2 tTexture2D<T>::getSize() const				{ return this->v2Size; }
template<typename T> color tTexture2D<T>::getPixel(int x, int y) const
{ 
    color retcol;
	if(x < 0 || x > v2Size.x || y < 0 || y > v2Size.y)
        return retcol;

    int pos = v2Size.x * y * this->numChannels(0) + x * this->numChannels(0);

    for(unsigned int i = 0; i < this->numChannels(0); i++)
        retcol[i] = this->getDataAt(pos + i, 0);
    return retcol;
}

template<typename T> SerializationData& tTexture2D<T>::serialize(SerializationData& data)
{
    Texture::serialize(data);
    int numchannels = this->iNumChannels[0];
    data & v2Size & numchannels;
    this->iNumChannels[0] = numchannels;


    if(data.isSavingData())
    {
        ImageData imagedata;
        imagedata.data = this->getDataPtr(0);
        imagedata.height = v2Size.y;
        imagedata.width = v2Size.x;
        imagedata.numComps = this->numChannels(0);

        unsigned int size = 0;
        unsigned char* jpgdata = TextureLoader::writeImageToMemory(GUM_TEXTURE_FILETYPE_JPG, imagedata, size, iSerializeQuality);
        
        data.addRawBytes(jpgdata, size);
    }
    else
    {
        unsigned int datasize = 0;
        unsigned char* jpgdata = nullptr;
        jpgdata = data.addRawBytes(jpgdata, datasize);
        
        ImageData imagedata = TextureLoader::loadImage(jpgdata, datasize);
        this->setDataPtr((T*)imagedata.data, 0);
        this->setNumChannels(imagedata.numComps, 0);
        v2Size = ivec2(imagedata.width, imagedata.height);
        updateImage();
    }


    return data;
}