#pragma once
#include <gum-maths.h>
#include <Essentials/Serialization.h>
#include <list>
#include <mutex>

class Texture : public Serialization
{
public:
    inline static const int MAX_PIXEL_COLOR = 256 * 256 * 256;
    inline static std::list<Texture*> vToLoadTextures;
    inline static std::mutex loadMutex;

    enum Type
    {
        TEXTURE2D,
        TEXTURE3D,
        TEXTURECUBE,
        TEXTUREHDR,
        TEXTUREDEPTH2D,
        TEXTUREDEPTH3D,
    };

    enum FilteringType
    {
        LINEAR,
        NEAREST_NEIGHBOR,
        LINEAR_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        NEAREST_MIPMAP_NEAREST,
    };

private:
    void createNative();
    void destroyNative();

protected:
	Texture(std::string name, Type type);

	unsigned int iTextureID;
	unsigned int iType;
	std::string sName;
	bool bLoaded;
    bool bIsGrayscale;
    bool bIsMipmapped;
    bool bHasTransparency;
    unsigned short iCurrentMipmapLevel;

public:
	virtual ~Texture();

    virtual void updateImage() { /* Empty */ }
	virtual void bind(const int& index = 0) {}
	virtual void unbind(const int& index = 0) {}

    static void loadTextures();
    
    void createMipmaps();
    virtual void repeat(bool mirrored = false) {}
    virtual void clampToEdge(bool border = false) {}
    virtual void setFiltering(FilteringType filteringtype) {}
    

	//Setter
	void setName(const std::string& name);
	void setID(const int& id);
    void markLoaded();
    void setGrayscale(const bool& isgrayscale);
    void setTransparency(const bool& hastransparency);
    void setActiveMipmapLevel(const unsigned short& level);

	//Getter
	Type getType() const;
	unsigned int getID() const;
	std::string getName() const;
	bool isLoaded() const;
    bool isGrayscale() const;
    bool hasTransparency() const;

    virtual SerializationData& serialize(SerializationData& data) override;
};