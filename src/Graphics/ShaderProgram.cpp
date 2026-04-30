#include "ShaderProgram.h"
#include "Essentials/Tools.h"
#include "Uniform.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>
#include <Essentials/Clock.h>

ShaderProgram::ShaderProgram(const std::string& name, bool internal)
  : sName(name),
    bIsInternal(internal)
{
  createNative();

  if(Tools::mapHasKeyNotNull(mShaderPrograms, name))
    Gum::Output::warn("ShaderProgram: program with name " + name + " already exists!");
  else
    mShaderPrograms[name] = this;

    
  if(pCurrentlyBoundShaderProgram == nullptr)
    pCurrentlyBoundShaderProgram = this;
}

ShaderProgram::~ShaderProgram() 
{
  if(pCurrentlyBoundShaderProgram == this)
    pCurrentlyBoundShaderProgram = nullptr;
  destroyNative();
  if(Tools::mapHasKey(mShaderPrograms, sName))
    mShaderPrograms.erase(sName);

  Locations.clear();
	vShaders.clear();
}

//Compiles the shaders into a form that your GPU can understand
void ShaderProgram::compileShaders() 
{
	for(Shader* shader : vShaders)
	{
    std::string error = shader->compile();
    if(error != "")
      Gum::Output::fatal("ShaderProgram: " + this->sName + ": " + shader->getShaderTypeStr() + ": " + error);
	}
}

void ShaderProgram::addUniform(const std::string& Name) { Locations[Name] = getUniformLocation(Name); }
void ShaderProgram::addUniform(const std::string& Name, const int& size)
{
	for (int i = 0; i < size; i++)
		addUniform(Name + "[" + std::to_string(i) + "]");
}
void ShaderProgram::addTexture(const std::string& Name, const int& index)
{
    Locations[Name] = getUniformLocation(Name);
    use();
    loadUniform(Name, index);
    unuse();
}


void ShaderProgram::addShader(Shader* shader) { if(shader != nullptr) this->vShaders.insert(shader); }
void ShaderProgram::removeShader(Shader* shader)   { this->vShaders.erase(shader); }

void ShaderProgram::build(std::map<const char*, unsigned int> attributes)
{
  if(bHasBeenBuilt)
    return;

  Clock benchmarkClock;
  //Gum::Output::debug("ShaderProgram: Creating Shader Program for " + sName);
	compileShaders();

	for (auto attribute : attributes)
	{
		addAttribute(attribute.first, attribute.second);
		//Gum::Output::debug("Adding attribute " + std::string(attribute.first) + " (" + std::to_string(attribute.second) + ")");
	}

	if(attributes.size() > 0)
		Gum::Output::debug("");

  Gum::Output::debug("ShaderProgram: Linking " + sName);
	linkShaders();

  //Adding default uniforms
  addUniform("canvassize");
  addUniform("viewMatrix");
  addUniform("projectionMatrix");

  int texnum = 0;
	for(Shader* shader : vShaders)
	{
    for(Uniform uniform : shader->getUniforms())
    {
      if(uniform.type != Uniform::UNKNOWN)
      {
        if(uniform.amount > 1)
            addUniform(uniform.name, uniform.amount);
        else
            addUniform(uniform.name);
      }
      if(uniform.type == Uniform::TEXTURE)
      {
        use();
        loadUniform(uniform.name, texnum++);
        unuse();
      }
    }
  }

  Gum::Output::debug("ShaderProgram: Building " + sName + " took " + std::to_string(benchmarkClock.getPassedTimeInMicros()) + "us");
  bHasBeenBuilt = true;
}

//
// Setter
//
void ShaderProgram::setName(const std::string& name) 				{ this->sName = name; }
void ShaderProgram::setCurrentlyBoundShader(ShaderProgram* program) { pCurrentlyBoundShaderProgram = program; }


//
// Getter
//
std::string ShaderProgram::getName() const           				                        { return this->sName; }
unsigned int ShaderProgram::getProgramID() const           			                    { return this->iProgramID; }
bool ShaderProgram::isInternal()                                                    { return this->bIsInternal; }
ShaderProgram* ShaderProgram::getCurrentlyBoundShader() 			                      { return pCurrentlyBoundShaderProgram; }
unsigned int ShaderProgram::numShaderPrograms()                                     { return (unsigned int)mShaderPrograms.size(); }
std::unordered_map<std::string, ShaderProgram*>& ShaderProgram::getShaderPrograms() { return mShaderPrograms; }
ShaderProgram* ShaderProgram::requestShaderProgram(const std::string& name, bool internal)
{ 
  if(Tools::mapHasKey(mShaderPrograms, name))
    return mShaderPrograms[name]; 
  
  return new ShaderProgram(name, internal);
}

Shader* ShaderProgram::getShader(unsigned int type)
{ 
	for(Shader* shader : vShaders)
    if(shader->getShaderType() == type)
      return shader;
  return nullptr;
}


void ShaderProgram::destroyAllShaders()
{
    while(mShaderPrograms.size() > 0)
        Gum::_delete(mShaderPrograms.begin()->second);

    Shader::destroyAllShaders();
}