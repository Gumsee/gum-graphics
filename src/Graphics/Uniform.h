#pragma once
#include "System/Output.h"
#include <string>
#include <Essentials/Tools.h>

struct Uniform
{
    enum Type
    {
        FLOAT,
        INTEGER,
        BOOL,
        VEC2, VEC3, VEC4,
        IVEC2, IVEC3, IVEC4,
        MAT2, MAT3, MAT4,
        TEXTURE,
        UNKNOWN
    };

    std::string name = "";
    unsigned short amount = 1;
    Type type = UNKNOWN;

    void strToType(std::string s)
    {
        if     (s == "int")   type = Uniform::INTEGER;
        else if(s == "float") type = Uniform::FLOAT;
        else if(s == "bool")  type = Uniform::BOOL;
        else if(s == "vec2")  type = Uniform::VEC2;
        else if(s == "vec3")  type = Uniform::VEC3;
        else if(s == "vec4")  type = Uniform::VEC4;
        else if(s == "ivec2") type = Uniform::IVEC2;
        else if(s == "ivec3") type = Uniform::IVEC3;
        else if(s == "ivec4") type = Uniform::IVEC4;
        else if(s == "mat2")  type = Uniform::MAT2;
        else if(s == "mat3")  type = Uniform::MAT3;
        else if(s == "mat4")  type = Uniform::MAT4;
        else if(Tools::strContains(s, "sampler"))  type = Uniform::TEXTURE;
        else Gum::Output::warn("Unknown uniform type: " + s + " for uniform " + name);
    }
};