#pragma once
#include "Uniform.h"
#include <string>
#include <vector>

#define GUM_SHADER_VERSION_STR "#version 450 core \n"
#define GLSL(str) GUM_SHADER_VERSION_STR #str

class Shader
{
public:
    struct TYPES 
    {
        static const unsigned int VERTEX_SHADER;
        static const unsigned int FRAGMENT_SHADER;
        static const unsigned int TESSELLATION_CONTROL_SHADER;
        static const unsigned int TESSELLATION_EVALUATION_SHADER;
        static const unsigned int GEOMETRY_SHADER;
    };

private:
    std::string sSource;
    std::string sTypeName;
    unsigned int iType;
    unsigned int iShaderID;
    bool bIsCompiled = false;
    std::vector<Uniform> vUniforms;

    void createNative();
    void destroyNative();

public:
    Shader(std::string sourcecode, unsigned int shadertype);
    ~Shader();

    std::string compile();

    //Getter
    std::string getSourceCode();
    unsigned int getShaderType();
    std::string getShaderTypeStr();
    unsigned int getShaderID();
    std::vector<Uniform>& getUniforms();

    //Setter
    void setSourceCode(std::string code);
};