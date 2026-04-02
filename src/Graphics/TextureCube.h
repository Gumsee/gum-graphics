#pragma once
#include "Texture2D.h"
#include "TextureImageData.h"

template<typename T>
class tTextureCube : public Texture, public TextureImageData<T, 6>
{
private:
    using Data = TextureImageData<T, 6>;
    
protected:
	ivec2 v2Size[6];

public:
	tTextureCube(std::string name = "unknown");
	~tTextureCube() {};

	void bind(const int& index = 0);
	void unbind(const int& index = 0);
	static void unbindGlobal(const int& index = 0);
    
    void repeat(bool mirrored = false);
    void clampToEdge(bool border = false);
    void setFiltering(FilteringType filteringtype);
	
    void updateImage();
    void updateImage(int side);
	void load(std::vector<std::string> texturepaths, bool wait);
	
    void setData(T* data, const unsigned int& side);
    void setSize(ivec2 size, int side);
    void setSize(ivec2 size);
};

typedef tTextureCube<unsigned char> TextureCube;
typedef tTextureCube<float> TextureCubef;