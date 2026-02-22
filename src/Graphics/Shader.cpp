#include "Shader.h"
#include "System/Output.h"
#include "Uniform.h"
#include <Essentials/Tools.h>

Shader::Shader(std::string sourcecode, unsigned int shadertype)
{
    this->sSource = sourcecode;
    this->iType = shadertype;

    for(std::string uniformentry : Tools::grep("uniform", sourcecode, ';'))
    {
        std::string uniformline = uniformentry.substr(uniformentry.find("uniform")); 
        std::vector<std::string> parts = Tools::splitStr(uniformline, ' ');
        if(parts.size() < 3)
        {
            Gum::Output::error("Uniform is invalid: " + uniformline);
            continue;
        }
        Uniform uniform;
        uniform.name = parts[2];
        uniform.strToType(parts[1]);

        if(Tools::strContains(uniform.name, "["))
        {
            std::vector<std::string> nameparts = Tools::splitStr(uniform.name, '[');
            uniform.name = nameparts[0];
            if(nameparts.size() > 1)
            {
                uniform.amount = Tools::StringToInt(Tools::strExtractNumbers(nameparts[1]));
            }
        }

        vUniforms.push_back(uniform);
    }

    createNative();
}

Shader::~Shader()
{
    destroyNative();
}


//
//Getter
//
std::string Shader::getSourceCode()          { return this->sSource; }
unsigned int Shader::getShaderType()         { return this->iType; }
unsigned int Shader::getShaderID()           { return this->iShaderID; }
std::vector<Uniform>& Shader::getUniforms()  { return this->vUniforms; }
std::string Shader::getShaderTypeStr()
{
    if     (iType == TYPES::VERTEX_SHADER)                  return "Vertex Shader";
    else if(iType == TYPES::FRAGMENT_SHADER)                return "Fragment Shader";
    else if(iType == TYPES::TESSELLATION_CONTROL_SHADER)    return "Tessellation Control Shader";
    else if(iType == TYPES::TESSELLATION_EVALUATION_SHADER) return "Tessellation Evaluation Shader";
    else if(iType == TYPES::GEOMETRY_SHADER)                return "Geometry Shader";
}


//
// Setter
//
void Shader::setSourceCode(std::string code) { this->sSource = code; }