#include "Framebuffer.h"
#include <System/MemoryManagement.h>
#include "WrapperFunctions.h"

Framebuffer::Framebuffer(const ivec2& size, bool iswindow)
{
    this->v2Offset = ivec2(0,0);
    this->v2Size = size;
    this->iID = 0;
    this->iDepthBufferID = 0;
    this->pDepthTexture = nullptr;
    this->bbRenderArea = bbox2i(ivec2(0,0), size);

    if(!iswindow)
    {
        createNative();
    }
    else if(WindowFramebuffer == nullptr)
    {
        WindowFramebuffer = this;
    }

    if(CurrentlyBoundFramebuffer == nullptr)
        CurrentlyBoundFramebuffer = this;
    updateMatrix();
}

Framebuffer::~Framebuffer() 
{
    destroyNative();
    for(auto entry : mTextureAttachments)
        Gum::_delete(entry.second);

    Gum::_delete(pDepthTexture);
}

void Framebuffer::updateMatrix()
{
    v2PixelSize = vec2(1.0f) / (vec2)v2Size;
    fAspectRatio = (float)v2Size.y / (float)v2Size.x;
    fAspectRatioWidthToHeight = (float)v2Size.x / (float)v2Size.y;
    m4ScreenMatrix = Gum::Maths::ortho((float)v2Size.y, (float)v2Size.x, 0.0f, 0.0f, -100.0f, 100.0f);
}


//
// Setter
//
void Framebuffer::setOffset(ivec2 offset)               { this->v2Offset = offset; }
void Framebuffer::setSize(ivec2 size)                   { this->v2Size = size; updateMatrix(); resizeTextures(); }
void Framebuffer::setClearColor(color clearcolor)       { this->cClearColor = clearcolor; }
void Framebuffer::setRenderArea(const bbox2i& area)     { this->bbRenderArea = area; gumScissor(area); }


//
// Getter
//
Texture* Framebuffer::getTextureAttachment(int index)   { return this->mTextureAttachments[index]; }
Texture* Framebuffer::getDepthTextureAttachment()       { return this->pDepthTexture; }
int Framebuffer::getDepthAttachmentID()                 { return this->iDepthBufferID; }
int Framebuffer::numTextureAttachments()                { return this->mTextureAttachments.size(); }
ivec2 Framebuffer::getSize()                            { return this->v2Size; }
ivec2 Framebuffer::getOffset()                          { return this->v2Offset; }
unsigned int Framebuffer::getID()                       { return this->iID; }
mat4 Framebuffer::getScreenMatrix()                     { return this->m4ScreenMatrix; }
float Framebuffer::getAspectRatio()        	            { return this->fAspectRatio; }
float Framebuffer::getAspectRatioWidthToHeight()        { return this->fAspectRatioWidthToHeight; }
vec2 Framebuffer::getPixelSize() const          		{ return this->v2PixelSize; }
color Framebuffer::getClearColor() const                { return this->cClearColor; }
bbox2i Framebuffer::getRenderArea() const               { return this->bbRenderArea; }