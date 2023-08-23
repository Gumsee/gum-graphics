#include "Shader.h"

Shader::Shader(std::string sourcecode, unsigned int shadertype)
{
    this->sSource = sourcecode;
    this->iType = shadertype;
    if     (iType == TYPES::VERTEX_SHADER)                  { this->sTypeName = "Vertex Shader"; }
    else if(iType == TYPES::FRAGMENT_SHADER)                { this->sTypeName = "Fragment Shader"; }
    else if(iType == TYPES::TESSELLATION_CONTROL_SHADER)    { this->sTypeName = "Tessellation Control Shader"; }
    else if(iType == TYPES::TESSELLATION_EVALUATION_SHADER) { this->sTypeName = "Tessellation Evaluation Shader"; }
    else if(iType == TYPES::GEOMETRY_SHADER)                { this->sTypeName = "Geometry Shader"; }
    createNative();
}

Shader::~Shader()
{
    destroyNative();
}

//Getter
std::string Shader::getSourceCode()  { return this->sSource; }
unsigned int Shader::getShaderType() { return this->iType; }
unsigned int Shader::getShaderID()   { return this->iShaderID; }