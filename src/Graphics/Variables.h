#pragma once
#include <cstdint>

namespace Gum 
{
    namespace Graphics
    {
        extern bool RUN_HEADLESS;
        
        struct Datatypes
        {
            static const uint16_t UNSIGNED_CHAR;
            static const uint16_t FLOAT;
            static const uint16_t INTEGER;
            static const uint16_t UNSIGNED_INTEGER;
        };
        
        struct DataState
        {
            static const unsigned short STATIC;
            static const unsigned short DYNAMIC;
        };

        struct Features
        {
            static const unsigned short SCISSOR_TEST;
            static const unsigned short BLENDING;
            static const unsigned short CULL_FACE;
            static const unsigned short DEPTH_TESTING;
            static const unsigned short STENCIL_TESTING;
            static const unsigned short FRAMEBUFFER_SRGB;
        };

        struct Pixelformat
        {
            static const uint16_t RGBA;
            static const uint16_t RGB;
            //static const uint16_t BGRA;
            //static const uint16_t BGR;
            static const uint16_t RG;
            static const uint16_t R;
            static const uint16_t RGBA16F;
            static const uint16_t RGBA32F;
        };
    }
}