#pragma once
#include "Uniform.h"
#include <string>
#include <vector>

#define GUM_SHADER_VERSION_STR "SHADER_VERSION_STRING"
#define _GLSL(str) #str
#define GLSL(str) GUM_SHADER_VERSION_STR "\n\n" _GLSL(str)

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
  std::string sName;
  unsigned int iType = 0;
  unsigned int iShaderID = 0;
  bool bIsCompiled = false;
  std::vector<Uniform> vUniforms;

  static inline std::unordered_map<std::string, Shader*> mShaders;

  void createNative();
  void destroyNative();
  
protected:
  Shader(std::string name, std::string sourcecode, unsigned int shadertype);

public:
  ~Shader();

  std::string compile();

  //Getter
  std::string getSourceCode();
  unsigned int getShaderType();
  std::string getShaderTypeStr();
  unsigned int getShaderID();
  std::vector<Uniform>& getUniforms();
	static Shader* requestShader(const std::string& name, std::string sourcecode, unsigned int shadertype);

  //Setter
  void setSourceCode(std::string code);
  
  static void destroyAllShaders();
};