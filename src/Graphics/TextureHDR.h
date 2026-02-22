#pragma once
#include "Texture2D.h"

class TextureHDR : public Texture2Df
{
public:
    TextureHDR(std::string name = "unknown")
        : Texture2Df(name)
    {
        this->iType = TEXTUREHDR;
        clampToEdge();
        setFiltering(LINEAR);
    }
};