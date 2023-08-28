#pragma once
#include <cstdint>

namespace Gum 
{
    namespace Graphics
    {
        struct Datatypes
        {
            static const uint16_t UNSIGNED_CHAR;
            static const uint16_t FLOAT;
            static const uint16_t INTEGER;
            static const uint16_t UINTEGER;
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
        };
    }
}