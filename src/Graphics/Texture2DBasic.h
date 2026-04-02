#pragma once
#include "Variables.h"
#include "Texture.h"

class Texture2DBasic
{
private:
	  ivec2 v2Size;
    unsigned int iChannels;
    unsigned int iTextureID;

    void createNative();
    void destroyNative();

public:
    Texture2DBasic();
    ~Texture2DBasic();
    void bind(const int& index = 0);
    void unbind(const int& index = 0);
    
    void repeat(bool mirrored = false);
    void clampToEdge(bool border = false);
    void setFiltering(Texture::FilteringType filteringtype);
    void updateImage(const ivec2& size, const int& numchannels, const void* data, uint16_t datatype);


    //Setter
    void setData(const ivec2& size, const int& numchannels, const void* data, uint16_t datatype);

    //Getter
    ivec2 getSize() const;
    unsigned int numChannels() const;
};
