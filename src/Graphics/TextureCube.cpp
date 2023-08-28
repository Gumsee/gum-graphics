#include "TextureCube.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <future>
#include <Codecs/TextureLoader.h>

TextureCube::TextureCube(std::string name, uint16_t datatype)
    : Texture(TEXTURECUBE, datatype)
{
	this->sName = name;
	for(int i = 0; i < 6; i++)
	{
		bNeedsFreeing[i] = false;
    	vPixelData[i] = nullptr;
		iChannels[i] = 0;
	}
    clampToEdge();
    setFiltering(LINEAR);
}

TextureCube::~TextureCube()
{
	for(int i = 0; i < 0; i++)
	{
		if(bNeedsFreeing[i] && vPixelData[i] != nullptr)
			free(vPixelData[i]);
	}
}

	
void TextureCube::updateImage()
{
	for(int i = 0; i < 6; i++)
        updateImage(i);
}


void TextureCube::load(std::vector<std::string> texturepaths, bool wait)
{
	if(texturepaths.size() != 6)
	{
		Gum::Output::error("TextureCube.load: Wrong amount of textures specified.");
		return;
	}

    auto future = std::async(std::launch::async, [texturepaths, this] {
		for(size_t i = 0; i < texturepaths.size(); i++)
		{
			ImageData imageData = TextureLoader::loadImage(texturepaths[i]);
			v2Size[i] = ivec2(imageData.width, imageData.height);
			iChannels[i] = imageData.numComps;
			vPixelData[i] = imageData.data;
			bNeedsFreeing[i] = true;
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
void TextureCube::setData(unsigned char* data, const unsigned int& side)
{
	vPixelData[side] = data;
}

void TextureCube::setSize(ivec2 size, int side)
{
    v2Size[side] = size;
    updateImage(side);
}

void TextureCube::setSize(ivec2 size)
{
    for(int i = 0; i < 6; i++)
        setSize(size, i);
}