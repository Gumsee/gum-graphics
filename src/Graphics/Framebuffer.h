#pragma once
#include <gum-maths.h>
#include "TextureDepth.h"
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
    color cClearColor;

    std::unordered_map<uint8_t, Texture*> mTextureAttachments;
    std::vector<unsigned int> vDrawBuffers;
    TextureDepth* pDepthTexture;
    mat4 m4ScreenMatrix;
    float fAspectRatio, fAspectRatioWidthToHeight;

    void createNative();
    void destroyNative();

    void checkStatus();
    void updateMatrix();
    void resizeTextures();

public:
    Framebuffer(const ivec2& size, bool iswindow = false);
    ~Framebuffer();

    inline static Framebuffer* CurrentlyBoundFramebuffer = nullptr;
    inline static Framebuffer* WindowFramebuffer = nullptr;

    void bind();
    static void unbind(const ivec2& viewportsize = ivec2(0,0));
    void resetViewport();
    void clear(const unsigned short& flags);

    Texture2D* addTextureAttachment(unsigned int index = 0, std::string name = "framebufferTexture", uint16_t datatype = Gum::Graphics::Datatypes::UNSIGNED_CHAR, uint16_t numChannels = 4);
    TextureCube* addCubeTextureAttachment(unsigned int index = 0, std::string name = "framebufferCubeTexture", uint16_t format = Texture::Pixelformat::RGBA, uint16_t internalFormat = Texture::Pixelformat::RGBA, int datatype = Gum::Graphics::Datatypes::UNSIGNED_CHAR);
    TextureCube* addCubeTextureAttachment(unsigned int index, TextureCube* texture);
    TextureDepth* addDepthTextureAttachment(std::string name = "framebufferDepthTexture");
    TextureDepth* addDepthStencilTextureAttachment(std::string name = "framebufferDepthStencilTexture");
    void addDepthAttachment();
    
    void attachTexture(const int& index, Texture* texture, const unsigned short& target, const unsigned int& mipmaplevel = 0U);
    void blitDepthToOtherFramebuffer(Framebuffer* fbo);


    //Setter
    void setDepthAttachment(unsigned int attachment);
    void setDepthTextureAttachment(TextureDepth* depthMap);
    void setOffset(ivec2 offset);
    void setSize(ivec2 size);
    void setClearColor(color clearcolor);

    //Getter
    Texture* getTextureAttachment(int index = 0);
    TextureDepth* getDepthTextureAttachment();
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
    color getClearColor() const;
};