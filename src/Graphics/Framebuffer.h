#pragma once
#include <gum-maths.h>
#include "Maths/bbox.h"
#include "TextureDepth2D.h"
#include "TextureDepth3D.h"
#include "TextureCube.h"
#include "Texture2D.h"
#include <unordered_map>
#include <vector>

class Framebuffer
{
public:
    struct ClearFlags
    {
        static const unsigned short COLOR;
        static const unsigned short DEPTH;
        static const unsigned short STENCIL;
    };
    
    struct TextureTargets
    {
        static const unsigned short TEXTURE2D;
        static const unsigned short CUBEMAP_POSITIVE_X;
        static const unsigned short CUBEMAP_NEGATIVE_X;
        static const unsigned short CUBEMAP_POSITIVE_Y;
        static const unsigned short CUBEMAP_NEGATIVE_Y;
        static const unsigned short CUBEMAP_POSITIVE_Z;
        static const unsigned short CUBEMAP_NEGATIVE_Z;
    };

private:
    unsigned int iID;
    unsigned int iDepthBufferID;
    ivec2 v2Size;
    ivec2 v2Offset;
    vec2 v2PixelSize;
    bbox2i bbRenderArea;
    color cClearColor;

    std::unordered_map<uint8_t, Texture*> mTextureAttachments;
    std::vector<unsigned int> vDrawBuffers;
    Texture* pDepthTexture;
    mat4 m4ScreenMatrix;
    float fAspectRatio, fAspectRatioWidthToHeight;

    void createNative();
    void destroyNative();

    void checkStatus(std::string funcname);
    void updateMatrix();
    void resizeTextures();

public:
    Framebuffer(const ivec2& size, bool iswindow = false, const unsigned int id = 0);
    ~Framebuffer();

    inline static Framebuffer* CurrentlyBoundFramebuffer = nullptr;
    inline static Framebuffer* WindowFramebuffer = nullptr;
    inline static unsigned int DefaultFramebufferID = 0;

    void bind();
    static void unbind(const ivec2& viewportsize = ivec2(0,0));
    void resetViewport();
    void clear(const unsigned short& flags);

    template<typename T> tTexture2D<T>* addTextureAttachment(unsigned int index = 0, std::string name = "framebufferTexture", uint16_t numChannels = 4);
    template<typename T> tTextureCube<T>* addCubeTextureAttachment(unsigned int index = 0, std::string name = "framebufferCubeTexture", uint16_t format = Gum::Graphics::Pixelformat::RGBA, uint16_t internalFormat = Gum::Graphics::Pixelformat::RGBA);
    template<typename T> tTextureCube<T>* addCubeTextureAttachment(unsigned int index, tTextureCube<T>* texture);
    Texture* addDepthTextureAttachment(std::string name = "framebufferDepthTexture");
    Texture* addDepthTextureArrayAttachment(const unsigned int& numlayers, std::string name = "framebufferDepthTextureArray");
    Texture* addDepthStencilTextureAttachment(std::string name = "framebufferDepthStencilTexture");
    void addDepthAttachment();
    
    void attachTexture(const int& index, Texture* texture, const unsigned short& target, const unsigned int& mipmaplevel = 0U);
    void blitDepthToOtherFramebuffer(Framebuffer* fbo);


    //Setter
    void setDepthAttachment(unsigned int attachment);
    void setDepthTextureAttachment(Texture* depthMap);
    void setOffset(ivec2 offset);
    void setSize(ivec2 size);
    void setClearColor(color clearcolor);
    void setRenderArea(const bbox2i& area);
    void overrideID(unsigned int id);


    //Getter
    Texture* getTextureAttachment(int index = 0);
    Texture* getDepthTextureAttachment();
    int getDepthAttachmentID();
    int numTextureAttachments();
    ivec2 getSize();
    ivec2 getOffset();
    unsigned int getID();
    mat4 getScreenMatrix();
    float getAspectRatio();
    float getAspectRatioWidthToHeight();
    color getPixel(ivec2 pos);
    vec2 getPixelSize() const;
    bool readPixelData(void* image, ivec2 position, ivec2 size, unsigned int pixelformat = Gum::Graphics::Pixelformat::RGBA, unsigned int datatype = Gum::Graphics::Datatypes::UNSIGNED_CHAR);
    color getClearColor() const;
    bbox2i getRenderArea() const;
};