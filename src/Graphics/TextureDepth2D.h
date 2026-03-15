#pragma once
#include "Texture.h"
#include "Variables.h"

class TextureDepth2D : public Texture
{
private:
    void createNative();
    
protected:
	ivec2 v2Size;
    uint16_t iDatatype;

public:
	TextureDepth2D(std::string name = "unknown", uint16_t datatype = Gum::Graphics::Datatypes::UNSIGNED_CHAR);
	~TextureDepth2D();

    void updateImage() override;
	void bind(const int& index = 0) override;
	void unbind(const int& index = 0) override;
    void repeat([[maybe_unused]] bool mirrored = false) override {};
    void clampToEdge([[maybe_unused]] bool border = false) override {};
    void setFiltering([[maybe_unused]] FilteringType filteringtype) override {};


    //Setter
    void setSize(const ivec2& size);

    //Getter
    ivec2 getSize() const;
};
