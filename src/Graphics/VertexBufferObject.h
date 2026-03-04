#pragma once
#include "Variables.h"
#include <cstddef>
#include <vector>


namespace Gum {
namespace Graphics {
namespace VertexBufferObject
{
    unsigned int createNative();
    void destroyNative(const unsigned int& id);
    void bind(const unsigned int& id);
    void unbind();

    void setDataNative(size_t datasize, const void* data, size_t length, const unsigned int& usage);
    void setSingleDataNative(size_t datasize, const void* data, const unsigned int& offset);
    void* getDataPointerNative(size_t datasize, const unsigned int& offset, const unsigned int& amount);
}}};

template<typename T>
class VertexBufferObject
{
private:
    unsigned int ivboID;
    unsigned int iLength;
    size_t iSizeOfType;

public:
    VertexBufferObject() : iSizeOfType(sizeof(T))
    {
        ivboID = Gum::Graphics::VertexBufferObject::createNative();
    }

    ~VertexBufferObject() 
    {
        Gum::Graphics::VertexBufferObject::destroyNative(ivboID);
    }

    void bind()
    {
        Gum::Graphics::VertexBufferObject::bind(ivboID);
    }
    
    void unbind()
    {
        Gum::Graphics::VertexBufferObject::unbind();
    }

    void setData(const std::vector<T>& data, const unsigned short& usage)
    {
        bind();
        Gum::Graphics::VertexBufferObject::setDataNative(iSizeOfType, data.data(), data.size(), usage);
        iLength = (unsigned int)data.size();
        unbind();
    }

    T* getDataPointer(const unsigned int& offset, const unsigned int& amount)
    {
        bind();
        T* data = (T*)Gum::Graphics::VertexBufferObject::getDataPointerNative(iSizeOfType, offset, amount);
        unbind();
        return data;
    }

    void setSingleData(const T& data, const unsigned int& offset)
    {
        bind();
        Gum::Graphics::VertexBufferObject::setSingleDataNative(iSizeOfType, &data, offset);
        unbind();
    }

    unsigned int getID() const     { return this->ivboID; }
    unsigned int getLength() const { return this->iLength; }
};