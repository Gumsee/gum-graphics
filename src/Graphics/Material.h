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
	std::string sName;

	float fReflectivity;
	float fRefractivity;
	float fSpecularity;
	float fRoughness;
	int iTextureMultiplier;
	int iNumUsableTextures;
	bool bTransparency;
	bool bFlipNormal;
	color vColor;

	//Texture related stuff
	std::unordered_map<unsigned int, Texture*> mTextures;	
	bool bHasNormalMap;
	bool bHasSpecularMap;
	bool bHasReflectionMap;
	bool bHasRefractionMap;
	bool bHasRoughnessMap;
	bool bHasDisplacementMap;
	bool bHasAmbientOcclusionMap;
	bool bHasBlendMap;
	bool bHasTexture;
    
public:
	Material();
    Material(Gum::Filesystem::File materialfile);
	~Material();

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
	Texture* getTexture(int index);
	int numTextures();

	void setName(std::string name);
	void setColor(color col);
	void setSpecularity(float specularity);
	void setRoughness(float roughness);
	void setReflectivity(float reflectivity);
	void setRefractivity(float refractivity);
	void setTextureMultiplier(float texMultiplier);
	void setIsTransparency(bool isTransparent);
	void flipNormals(bool shouldFlip = true);
	void setTexture(Texture *tex, int index);
	void delTexture(int index);
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

    void onDeserialize() override;
    SerializationData& serialize(SerializationData& data) override;
};