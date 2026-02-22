#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include "System/Output.h"
#include "Variables.h"

template<typename T, unsigned NUM_DATA_ARRAYS>
class TextureImageData
{
static constexpr bool isValidType()
{
    return std::is_same<T, unsigned char>() 
        || std::is_same<T, char>()
        || std::is_same<T, unsigned short>()
        || std::is_same<T, short>()
        || std::is_same<T, unsigned int>()
        || std::is_same<T, int>()
        || std::is_same<T, float>();
}
static_assert(isValidType(), "Invalid type for Texture!");

private:
	bool bNeedsFreeing[NUM_DATA_ARRAYS];
    T* vPixelData[NUM_DATA_ARRAYS];
    
    const uint16_t iDatatype = 
          std::is_same<T, unsigned char>() ? Gum::Graphics::Datatypes::UNSIGNED_CHAR 
        : std::is_same<T, int>() ? Gum::Graphics::Datatypes::INTEGER 
        : std::is_same<T, unsigned int>() ? Gum::Graphics::Datatypes::UNSIGNED_INTEGER 
        : std::is_same<T, float>() ? Gum::Graphics::Datatypes::FLOAT 
        : 0;

protected:
    unsigned int iNumChannels[NUM_DATA_ARRAYS];

    void setDataPtr(T* data, size_t index)                { vPixelData[index] = data; }
    void setDataAt(size_t pos, size_t index, T data)      { vPixelData[index][pos] = data; }
    void setNeedsFreeing(bool needsfreeing, size_t index) { this->bNeedsFreeing[index] = needsfreeing; }

    T getDataAt(size_t pos, size_t index) const           { return this->vPixelData[index][pos]; }
    bool needsFreeing(size_t index) const                 { return this->bNeedsFreeing[index]; }

public:
    TextureImageData()
    {
        for(unsigned int i = 0; i < NUM_DATA_ARRAYS; i++)
        {
            bNeedsFreeing[i] = false;
            vPixelData[i] = nullptr;
            iNumChannels[i] = 4;
        }
    }

    ~TextureImageData()
    {
        for(unsigned int i = 0; i < NUM_DATA_ARRAYS; i++)
        {
            if(bNeedsFreeing[i] && vPixelData[i] != nullptr)
                free(vPixelData[i]);
        }
    }

    void setNumChannels(unsigned int numchannels, size_t index = 0)
    { 
        if(numchannels > 4)
        {
            Gum::Output::error("Invalid number of channels");
            return;
        }

        this->iNumChannels[index] = numchannels; 
    }

    T* getDataPtr(size_t index = 0) const                               { return this->vPixelData[index]; }
    unsigned int numChannels(size_t index = 0) const                    { return this->iNumChannels[index]; }
    uint16_t getDatatype() const                                        { return this->iDatatype; }
};