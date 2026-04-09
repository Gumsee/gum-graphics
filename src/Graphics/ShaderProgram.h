#pragma once
#include <Essentials/Settings.h>
#include <gum-maths.h>
#include "Shader.h"
#include <unordered_map>
#include <vector>

class ShaderProgram
{
private:
	static inline std::unordered_map<std::string, ShaderProgram*> mShaderPrograms;

	void getAllUniformLocations();

	std::vector<Shader*> vShaders;
	std::unordered_map<std::string, int> Locations;
	std::unordered_map<std::string, int> Attributes;
  
	std::string sName;
  bool bIsInternal;
	unsigned int iProgramID = 0;

	void compileShaders();
	void linkShaders();
	void createNative();
	void destroyNative();
	void addUniform(const std::string& Name);
	void addUniform(const std::string& Name, const int& size);

	inline static ShaderProgram* pCurrentlyBoundShaderProgram = nullptr;

public:
	ShaderProgram(const std::string& name, bool internal);
	~ShaderProgram();

	void addAttribute(const std::string& attributeName, const int& number);
	void addTexture(const std::string& Name, const int& index);


	void use();
	static void unuse();

	void loadUniform(const std::string& uniformName, const bool& var);
	void loadUniform(const std::string& uniformName, const vec2& var);
	void loadUniform(const std::string& uniformName, const ivec2& var);
	void loadUniform(const std::string& uniformName, const vec3& var);
	void loadUniform(const std::string& uniformName, const ivec3& var);
	void loadUniform(const std::string& uniformName, const vec4& var);
	void loadUniform(const std::string& uniformName, const mat4& var);
	void loadUniform(const std::string& uniformName, const color& var);
	void loadUniform(const std::string& uniformName, const float& var);
	void loadUniform(const std::string& uniformName, const int& var);

	void loadUniform(const std::string& uniformName, const std::vector<vec2>& var);
	void loadUniform(const std::string& uniformName, const std::vector<ivec2>& var);
	void loadUniform(const std::string& uniformName, const std::vector<vec3>& var);
	void loadUniform(const std::string& uniformName, const std::vector<ivec3>& var);
	void loadUniform(const std::string& uniformName, const std::vector<vec4>& var);
	void loadUniform(const std::string& uniformName, const std::vector<mat4>& var);
	void loadUniform(const std::string& uniformName, const std::vector<float>& var);
	void loadUniform(const std::string& uniformName, const std::vector<int>& var);

	int getUniformLocation(const std::string& UniformName);

	void build(std::map<const char*, unsigned int> attributes = {
		{"vertexPosition", 0}, {"TextureCoords", 1}, {"Normals", 2}, {"TransMatrix", 3}, 
		{"tangentNormals", 7}, {"jointIndices", 8},  {"weights", 9}, {"individualColor", 10}
	});
	void rebuild();

	void addShader(Shader* shader);
	void removeShader(int index);

    template<typename T>
    static void loadUniformForAll(const std::string& uniformName, const T& value)
    {
        ShaderProgram* current = getCurrentlyBoundShader();
        for(auto it : mShaderPrograms)
        {
            it.second->use();
            it.second->loadUniform(uniformName, value);
        }

        if(current != nullptr)
            current->use();
    }

  static void destroyAllShaders();


	//Setter
	void setName(const std::string& name);
	static void setCurrentlyBoundShader(ShaderProgram* program);

	//Getter
	std::string getName() const;
	unsigned int getProgramID() const;
	static ShaderProgram* getCurrentlyBoundShader();
	static ShaderProgram* getShaderProgramByName(const std::string& name);
	static std::unordered_map<std::string, ShaderProgram*>& getShaderPrograms();
	static unsigned int numShaderPrograms();
	Shader* getShader(int index);
    bool isInternal();
};

