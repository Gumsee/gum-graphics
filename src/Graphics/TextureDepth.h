#pragma once
#include "Texture2D.h"

class TextureDepth : public Texture
{
private:
    void createNative();
    
protected:
	ivec2 v2Size;

public:
	TextureDepth(std::string name = "unknown", uint16_t datatype = Datatypes::UNSIGNED_CHAR);
	~TextureDepth();

    void updateImage() override;
	void bind(const int& index = 0) override;
	void unbind(const int& index = 0) override;


    //Setter
    void setSize(const ivec2& size);

    //Getter
    ivec2 getSize() const;
};
