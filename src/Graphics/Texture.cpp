#include "Texture.h"
#include <Essentials/Tools.h>
#include "Texture2D.h"
#include "TextureHDR.h"

Texture::Texture(std::string name, Type type)
{
    this->sName = name;
    this->iType = type;
    this->bLoaded = false;
    this->bIsGrayscale = false;
    this->bHasTransparency = true;
    this->bIsMipmapped = false;
	  this->iTextureID = 0;
    this->iCurrentMipmapLevel = 0;
    createNative();
}

Texture::~Texture()
{
    destroyNative();
}

Texture* Texture::autoLoad(Gum::File filepath, bool waitForLoading)
{
    std::string name = filepath.toString();
    if(name.empty())
      return nullptr;
		std::string extension = "";
    if(name.length() > 3)
      Tools::toUpperCase(name.substr(name.length() - 3, 3));
		if(extension == "HDR")
		{
			TextureHDR *tex = new TextureHDR(name);
			tex->load(name, waitForLoading);
			return tex;
		}
		else
		{
			Texture2D *tex = new Texture2D(name);
			tex->load(name, waitForLoading);
			return tex;
		}
}

void Texture::updateBackgroundLoading()
{
    std::lock_guard<decltype(loadMutex)> lock(loadMutex);
    for(Texture* tex : vToLoadTextures)
        tex->updateImage();
    vToLoadTextures.clear();
}

void Texture::cleanupAllLoadedTextures()
{
  for(auto tex : mLoadedTextures)
    Gum::_delete(tex.second);
  mLoadedTextures.clear();
}

void Texture::createMipmaps()
{
    this->bIsMipmapped = true;
    updateImage();
}



//
// Setter
//
void Texture::setName(const std::string& name)	                { this->sName = name; }
void Texture::setID(const int& id) 				                { this->iTextureID = id; }
void Texture::markLoaded()                                      { this->bLoaded = true; }
void Texture::setGrayscale(const bool& isgrayscale)             { this->bIsGrayscale = isgrayscale; }
void Texture::setActiveMipmapLevel(const unsigned short& level) { this->iCurrentMipmapLevel = level; }
void Texture::setTransparency(const bool& hastransparency)      { this->bHasTransparency = hastransparency; }


//
// Getter
//
unsigned int Texture::getID() const    { return this->iTextureID; }
Texture::Type Texture::getType() const { return static_cast<Type>(this->iType); }
bool Texture::isLoaded() const 		   { return this->bLoaded; }
std::string Texture::getName() const   { return this->sName; }
bool Texture::isGrayscale() const      { return this->bIsGrayscale; }
bool Texture::hasTransparency() const  { return this->bHasTransparency; }


SerializationData& Texture::serialize(SerializationData& data)
{
    data & sName;
    return data;
}