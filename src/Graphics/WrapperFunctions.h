#pragma once
#include <gum-maths.h>

extern bool gumTexImage2D(const unsigned int& target, const int& level, const int& internalformat, ivec2 size, const int& border, const unsigned int& format, const unsigned int& type, const void* pixels);
extern bool gumPixelStorei(const unsigned int& pname, const int& param);
extern bool gumGenTextures(const int& n, unsigned int* textures);
extern bool gumScissor(bbox2i bbox);
extern std::string graphicsErrorCodeToString(const unsigned int& error);