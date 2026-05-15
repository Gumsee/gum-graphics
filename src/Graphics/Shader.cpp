#include "Shader.h"
#include "System/Output.h"
#include "Uniform.h"
#include <Essentials/Tools.h>
#include <System/MemoryManagement.h>

static inline std::string typeToString(const unsigned int& type)
{
  if     (type == Shader::TYPES::VERTEX_SHADER)                  return "Vertex";
  else if(type == Shader::TYPES::FRAGMENT_SHADER)                return "Fragment";
  else if(type == Shader::TYPES::TESSELLATION_CONTROL_SHADER)    return "Tesselation_Control";
  else if(type == Shader::TYPES::TESSELLATION_EVALUATION_SHADER) return "Tesselation_Evaluation";
  else if(type == Shader::TYPES::GEOMETRY_SHADER)                return "Geometry";

  return "";
}

Shader::Shader(std::string name, std::string sourcecode, unsigned int shadertype)
{
    this->sSource = sourcecode;
    this->iType = shadertype;
    this->sName = name;

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
              uniform.amount = Tools::StringToNum<short>(Tools::strExtractNumbers(nameparts[1]));
        }

        vUniforms.push_back(uniform);
    }

    createNative();
    
    if(Tools::mapHasKeyNotNull(mShaders, sName))
        Gum::Output::error("Shader: shader with id " + std::to_string(iShaderID) + " already exists!");
    else
        mShaders[sName] = this;
}

Shader::~Shader()
{
  destroyNative();
  if(Tools::mapHasKey(mShaders, sName))
    mShaders.erase(sName);
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

    return "Unknown Shader";
}
Shader* Shader::requestShader(const std::string& name, std::string sourcecode, unsigned int shadertype)
{ 
  std::string shadername = name + typeToString(shadertype);
  if(Tools::mapHasKey(mShaders, shadername))
    return mShaders[shadername];
  
  return new Shader(shadername, sourcecode, shadertype);
}


//
// Setter
//
void Shader::setSourceCode(std::string code) { this->sSource = code; }

void Shader::destroyAllShaders()
{
  while(mShaders.size() > 0)
    Gum::_delete(mShaders.begin()->second);
}