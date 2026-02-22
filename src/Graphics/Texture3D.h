#pragma once
#include "Texture.h"
#include "TextureImageData.h"
#include <functional>

typedef std::function<color(ivec3 pixelcoord)> Texture3DGeneratorFunc;

template<typename T>
class tTexture3D : public Texture, public TextureImageData<T, 1>
{
private:
    using Data = TextureImageData<T, 1>;

protected:
	ivec3 v3Size;

public:	
	tTexture3D(std::string name = "unknown");
	tTexture3D(const std::string& name, const ivec3& size, Texture3DGeneratorFunc generator);
    ~tTexture3D() override;

    void updateImage() override;
	void generate(Texture3DGeneratorFunc function);
	void bind(const int& index = 0) override;
	void unbind(const int& index = 0) override;
	static void unbindGlobal(const int& index = 0);

    void initEmpty();
    
    void repeat(bool mirrored = false) override;
    void clampToEdge(bool border = false) override;
    void setFiltering(FilteringType filteringtype) override;


    //Setter
    void setSize(const ivec3& size);
    void setData(T* data);
    void setPixel(const ivec3& coord, const color& col);

    //Getter
    ivec3 getSize() const;
    color getPixel(const ivec3& coord) const;
};

template class tTexture3D<unsigned char>;
template class tTexture3D<float>;

typedef tTexture3D<unsigned char> Texture3D;
typedef tTexture3D<float> Texture3Df;