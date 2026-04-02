#include "TextureCube.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <future>
#include <Codecs/TextureLoader.h>

template<typename T>
tTextureCube<T>::tTextureCube(std::string name)
    : Texture(name, TEXTURECUBE)
{
	this->sName = name;
    clampToEdge();
    setFiltering(LINEAR);
}

template<typename T>
void tTextureCube<T>::updateImage()
{
	for(int i = 0; i < 6; i++)
        updateImage(i);
}


template<typename T>
void tTextureCube<T>::load(std::vector<std::string> texturepaths, bool wait)
{
	if(texturepaths.size() != 6)
	{
		Gum::Output::error("tTextureCube.load: Wrong amount of textures specified.");
		return;
	}

    auto future = std::async(std::launch::async, [texturepaths, this] {
		for(size_t i = 0; i < texturepaths.size(); i++)
		{
			ImageData imageData = TextureLoader::loadImage(texturepaths[i]);
			v2Size[i] = ivec2(imageData.width, imageData.height);
            Data::setNumChannels(imageData.numComps, i);
            Data::setDataPtr((T*)imageData.data, i);
            Data::setNeedsFreeing(true, i);
		}

        vToLoadTextures.push_back(this);
        markLoaded();
    });



    if(wait)
        future.wait();
}



//
// Setter
//
template<typename T>
void tTextureCube<T>::setData(T* data, const unsigned int& side)
{
    Data::setDataPtr(data, side);
}

template<typename T>
void tTextureCube<T>::setSize(ivec2 size, int side)
{
    v2Size[side] = size;
    updateImage(side);
}

template<typename T>
void tTextureCube<T>::setSize(ivec2 size)
{
    for(int i = 0; i < 6; i++)
        setSize(size, i);
}

template class tTextureCube<unsigned char>;
template class tTextureCube<float>;
