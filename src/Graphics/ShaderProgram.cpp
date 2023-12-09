#include "ShaderProgram.h"
#include <System/Output.h>
#include <System/MemoryManagement.h>

ShaderProgram::ShaderProgram(bool internal)  
{
    bIsInternal = internal;
    createNative();
}

ShaderProgram::~ShaderProgram() 
{
    destroyNative();
	
	for(size_t i = 0; i < vShaders.size(); i++)
		Gum::_delete(vShaders[i]);

	Locations.clear();
	vShaders.clear();
}

//Compiles the shaders into a form that your GPU can understand
void ShaderProgram::compileShaders() 
{
	for(size_t i = 0; i < vShaders.size(); i++)
	{
        Gum::Output::debug("ShaderProgram: Compiling " + std::to_string(vShaders[i]->getShaderType()));
        std::string error = vShaders[i]->compile();
        if(error != "")
        {
            Gum::Output::fatal("ShaderProgram: " + this->sName + ": " + error);
        }
	}
}

void ShaderProgram::addUniform(const std::string& Name) { Locations[Name] = getUniformLocation(Name); }
void ShaderProgram::addUniform(const std::string& Name, const int& size)
{
	for (int i = 0; i < size; i++)
	{
		addUniform(Name + "[" + std::to_string(i) + "]");
	}
}
void ShaderProgram::addTexture(const std::string& Name, const int& index)
{
    Locations[Name] = getUniformLocation(Name);
    use();
    loadUniform(Name, index);
    unuse();
}


void ShaderProgram::addShader(Shader* shader) { this->vShaders.push_back(shader); }
void ShaderProgram::removeShader(int index)   { this->vShaders.erase(vShaders.begin() + index); }

void ShaderProgram::build(const std::string& name, std::map<const char*, unsigned int> attributes)
{
	this->sName = name;
    Gum::Output::debug("ShaderProgram: Creating Shader Program for " + sName);
	compileShaders();

	for (auto attribute : attributes)
	{
		addAttribute(attribute.first, attribute.second);
		Gum::Output::debug("Adding attribute " + std::string(attribute.first) + " (" + std::to_string(attribute.second) + ")");
	}

	if(attributes.size() > 0)
	{
		Gum::Output::debug("");
	}

    Gum::Output::debug("ShaderProgram: Linking " + name);
	linkShaders();


    Gum::Output::debug("ShaderProgram: Adding default Uniforms " + name);
	addUniform("transformationMatrix");
	addUniform("viewMatrix");
	addUniform("projectionMatrix");
}

//
// Setter
//
void ShaderProgram::setName(const std::string& name) 				{ this->sName = name; }
void ShaderProgram::setCurrentlyBoundShader(ShaderProgram* program) { pCurrentlyBoundShaderProgram = program; }


//
// Getter
//
std::string ShaderProgram::getName() const           				{ return this->sName; }
unsigned int ShaderProgram::getProgramID() const           			{ return this->iProgramID; }
Shader* ShaderProgram::getShader(int index) 						{ return this->vShaders[index]; }
bool ShaderProgram::isInternal()                                    { return this->bIsInternal; }
ShaderProgram* ShaderProgram::getCurrentlyBoundShader() 			{ return pCurrentlyBoundShaderProgram; }