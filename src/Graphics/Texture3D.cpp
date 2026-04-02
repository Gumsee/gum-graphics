#include "Texture3D.h"
#include "System/Output.h"
#include <cstdint>
#include <iostream>
#include <future>

template<typename T>
tTexture3D<T>::tTexture3D(std::string name)
    : Texture(name, TEXTURE3D)
{
	this->sName = name;
    if(!Gum::Graphics::RUN_HEADLESS)
    {
        repeat();
        setFiltering(FilteringType::LINEAR);
    }
}

template<typename T>
tTexture3D<T>::tTexture3D(const std::string& name, const ivec3& size, Texture3DGeneratorFunc generator)
    : tTexture3D(name)
{
    v3Size = size;
    initEmpty();
    this->generate(generator);
}
   
template<typename T> 
void tTexture3D<T>::initEmpty()
{
    size_t dataSize = v3Size.x * v3Size.y * v3Size.z * this->numChannels(0) * sizeof(T);
    this->setDataPtr((T*)malloc(dataSize), 0);
    memset(this->getDataPtr(0), 0, dataSize);
}

template<typename T>
void tTexture3D<T>::generate(Texture3DGeneratorFunc function)
{
    for(int x = 0; x < v3Size.x; x++)
    {
        for(int y = 0; y < v3Size.y; y++)
        {
            for(int z = 0; z < v3Size.z; z++)
            {
                color col = function(ivec3(x, y, z));
                //Layers + Lines + Collumn
                unsigned int pos = (v3Size.x * v3Size.y * z + v3Size.x * y + x) * this->numChannels(0);

                for(unsigned int i = 0; i < this->numChannels(0); i++)
                    this->setDataAt(pos + i, 0, (T)col[i]);
            }
        }
    }
    updateImage();
}


//
// Setter
//
template<typename T> void tTexture3D<T>::setData(T* data)  { this->setDataPtr(data, 0); updateImage(); }
template<typename T> void tTexture3D<T>::setSize(const ivec3& size)
{ 
    /*size_t oldDataSize = v2Size.x * v2Size.y * iChannels;
    size_t newDataSize = size.x * size.y * iChannels;

    vPixelData = realloc(vPixelData, newDataSize);

    if(oldDataSize < newDataSize) // Add data
        memset(vPixelData, oldDataSize, newDataSize - oldDataSize);*/

    this->v3Size = size; 
    updateImage(); 
}

template<typename T>
void tTexture3D<T>::setPixel(const ivec3& coord, const color& col) 
{
    unsigned int pos = (v3Size.x * v3Size.y * coord.z + v3Size.x * coord.y + coord.x) * this->numChannels(0);
    if(pos >= v3Size.x * v3Size.y * v3Size.z * this->numChannels(0))
        return;
    for(unsigned int i = 0; i < this->numChannels(0); i++)
        this->setDataAt(pos + i, 0, (T)col[i]);
}


//
// Getter
//
template<typename T> ivec3 tTexture3D<T>::getSize() const				{ return this->v3Size; }
template<typename T> color tTexture3D<T>::getPixel(const ivec3& coord) const
{ 
    color retcol;
	if(coord.x < 0 || coord.x > v3Size.x || coord.y < 0 || coord.y > v3Size.y || coord.z < 0 || coord.z > v3Size.z)
        return retcol;

    unsigned int pos = (v3Size.x * v3Size.y * coord.z + v3Size.x * coord.y + coord.x) * this->numChannels(0);

    for(unsigned int i = 0; i < this->numChannels(0); i++)
        retcol[i] = this->getDataAt(pos + i, 0);
    return retcol;
}

template class tTexture3D<unsigned char>;
template class tTexture3D<float>;
