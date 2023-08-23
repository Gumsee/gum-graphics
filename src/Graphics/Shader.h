#pragma once
#include <string>

class Shader
{
public:
    inline static const std::string SHADER_VERSION_STR = "#version 450 core \n";
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

    void createNative();
    void destroyNative();

public:
    Shader(std::string sourcecode, unsigned int shadertype);
    ~Shader();

    std::string compile();

    //Getter
    std::string getSourceCode();
    unsigned int getShaderType();
    unsigned int getShaderID();
};