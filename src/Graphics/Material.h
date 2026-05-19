#pragma once
#include <string>
#include <unordered_map>
#include <System/File.h>
#include <Codecs/Filetypes.h>
#include <Essentials/Serialization.h>
#include "Texture.h"

#define GUM_MATERIAL_NORMAL_MAP            14
#define GUM_MATERIAL_DISPLACEMENT_MAP      13
#define GUM_MATERIAL_BLEND_MAP             12
#define GUM_MATERIAL_REFLECTION_MAP        11
#define GUM_MATERIAL_REFRACTION_MAP        10
#define GUM_MATERIAL_SPECULAR_MAP          9
#define GUM_MATERIAL_ROUGHNESS_MAP         8
#define GUM_MATERIAL_AMBIENT_OCCLUSION_MAP 7
#define GUM_MATERIAL_TEXTURE0              0

class Material : public Serialization
{
private:
  static inline std::unordered_map<std::string, Material*> mMaterials;
  static inline Material* pDefaultMaterial = nullptr;

	std::string sName;

	float fReflectivity = 0.0f;
	float fRefractivity = 0.0f;
	float fSpecularity = 0.0f;
	float fRoughness = 0.0f;
	int iTextureMultiplier = 1;
	bool bTransparency = false;
	bool bFlipNormal = false;
	color vColor;

	//Texture related stuff
	std::unordered_map<unsigned int, Texture*> mTextures;	
	bool bHasNormalMap = false;
	bool bHasSpecularMap = false;
	bool bHasReflectionMap = false;
	bool bHasRefractionMap = false;
	bool bHasRoughnessMap = false;
	bool bHasDisplacementMap = false;
	bool bHasAmbientOcclusionMap = false;
	bool bHasBlendMap = false;
	bool bHasTexture = false;

protected:
  Material(std::string name);
  Material(std::string name, Gum::Filesystem::File materialfile);
    
public:
  virtual ~Material();

  void saveToFile(const Gum::Filesystem::File& file, const unsigned int& filetype = GUM_TEXTURE_FILETYPE_JPG);

	bool isReflective();
	bool isRefractive();
	bool isTransparent();
	bool hasFlippedNormals();
	float& getSpecularity();
	float& getRoughness();
	float& getReflectivity();
	float& getRefractivity();
	int getTextureMultiplier();
	std::string getName();
	color getColor();
	Texture* getTexture(unsigned int index);
	unsigned int numTextures();

	void setColor(color col);
	void setSpecularity(float specularity);
	void setRoughness(float roughness);
	void setReflectivity(float reflectivity);
	void setRefractivity(float refractivity);
	void setTextureMultiplier(int texMultiplier);
	void setIsTransparency(bool isTransparent);
	void flipNormals(bool shouldFlip = true);
	void setTexture(Texture *tex, unsigned int index);
	void delTexture(unsigned int index);
	void bindTextures();
	void unbindTextures();


	bool hasNormalMap();
	bool hasSpecularMap();
	bool hasReflectionMap();
	bool hasRefractionMap();
	bool hasRoughnessMap();
	bool hasDisplacementMap();
	bool hasAmbientOcclusionMap();
	bool hasBlendMap();
	bool hasTexture();

  static Material* getDefaultMaterial();

	static Material* requestMaterial(const std::string& name);
	static Material* requestMaterial(const std::string& name, Gum::Filesystem::File materialfile);
  static void destroyAllMaterials();

  void onDeserialize() override;
  SerializationData& serialize(SerializationData& data) override;
};