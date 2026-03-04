#pragma once
#include "Texture.h"
#include "TextureImageData.h"
#include <functional>
#include <System/File.h>

typedef std::function<color(ivec2 pixelcoord)> Texture2DGeneratorFunc;

template<typename T>
class tTexture2D : public Texture, public TextureImageData<T, 1>
{
protected:
    static inline unsigned int iSerializeQuality = 85;
	ivec2 v2Size;

public:
	tTexture2D(std::string name = "unknown");
	tTexture2D(const std::string& name, const ivec2& size, Texture2DGeneratorFunc generator);
	virtual ~tTexture2D() override;

    void updateImage() override;
	void load(const Gum::File& filepath, bool wait);
	void loadFromMemory(unsigned char* pixels, size_t size);
	void generate(Texture2DGeneratorFunc function);
	void bind(const int& index = 0) override;
	void unbind(const int& index = 0) override;
	static void unbindGlobal(const int& index = 0);

    float getHeightMapPixel(int x, int y);
    void initEmpty();
    
    void repeat(bool mirrored = false) override;
    void clampToEdge(bool border = false) override;
    void setFiltering(FilteringType filteringtype) override;


    //Setter
    void setData(T* data);
    void setSize(const ivec2& size);
    void setPixel(const int& x, const int& y, const color& col);
    static void setSerializeQuality(const unsigned int& quality);

    //Getter
    ivec2 getSize() const;
    color getPixel(int x, int y) const;

    SerializationData& serialize(SerializationData& data) override;
};

template class tTexture2D<unsigned char>;
template class tTexture2D<float>;

typedef tTexture2D<unsigned char> Texture2D;
typedef tTexture2D<float> Texture2Df;