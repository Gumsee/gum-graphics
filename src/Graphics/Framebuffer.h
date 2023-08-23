#pragma once
#include <gum-maths.h>
#include "TextureDepth.h"
#include "TextureCube.h"
#include <unordered_map>
#include <vector>

class Framebuffer
{
private:
    unsigned int iID;
    unsigned int iDepthBufferID;
    ivec2 v2Size;
    ivec2 v2Offset;
    vec2 v2PixelSize;

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

    Texture2D* addTextureAttachment(unsigned int index = 0, std::string name = "framebufferTexture", uint16_t datatype = Texture::Datatypes::UNSIGNED_CHAR, uint16_t numChannels = 4);
    TextureCube* addCubeTextureAttachment(unsigned int index = 0, std::string name = "framebufferCubeTexture", uint16_t format = Texture::Pixelformat::RGBA, uint16_t internalFormat = Texture::Pixelformat::RGBA, int datatype = Texture::Datatypes::UNSIGNED_CHAR);
    TextureDepth* addDepthTextureAttachment(std::string name = "framebufferDepthTexture");
    TextureDepth* addDepthStencilTextureAttachment(std::string name = "framebufferDepthStencilTexture");
    void addDepthAttachment();
    
    void drawAttachmentTexture(const int& textureattachmentindex, const int& colorattachmentindex, const int& target);
    void blitDepthToOtherFramebuffer(Framebuffer* fbo);


    //Setter
    void setDepthAttachment(unsigned int attachment);
    void setDepthTextureAttachment(TextureDepth* depthMap);
    void setOffset(ivec2 offset);
    void setSize(ivec2 size);

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
    vec4 getPixel(ivec2 pos);
    vec2 getPixelSize() const;
};