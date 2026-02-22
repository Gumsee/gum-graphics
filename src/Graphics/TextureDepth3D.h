#pragma once
#include "Texture.h"
#include "Variables.h"

class TextureDepth3D : public Texture
{
private:
    void createNative();
    
protected:
	ivec2 v2Size;
    unsigned int iNumLayers;
    uint16_t iDatatype;

public:
	TextureDepth3D(const unsigned int& numlayers, std::string name = "unknown", uint16_t datatype = Gum::Graphics::Datatypes::UNSIGNED_CHAR);
	~TextureDepth3D();

    void updateImage() override;
	void bind(const int& index = 0) override;
	void unbind(const int& index = 0) override;


    //Setter
    void setSize(const ivec2& size);
    void setBordercolor(const color& col);

    //Getter
    ivec2 getSize() const;
    unsigned int numLayers() const;
};
