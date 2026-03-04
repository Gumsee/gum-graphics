#include "Material.h"
#include <Essentials/Serialization.h>
#include <Essentials/Tools.h>
#include "System/Filesystem.h"
#include "Texture2D.h"
#include <System/File.h>
#include <System/Output.h>
#include <Codecs/Zip.h>


Material::Material()
{
	sName = "";
	fReflectivity = 0.0f;
	fRefractivity = 0.0f;
	fSpecularity = 0;//0.3f;
	fRoughness = 0;//0.8f;
	iTextureMultiplier = 1;
	bTransparency = false;
	bFlipNormal = false;
	vColor = rgba(100, 100, 100, 255);

	iNumUsableTextures = 14;

	bHasNormalMap = false;
	bHasSpecularMap = false;
	bHasReflectionMap = false;
	bHasRefractionMap = false;
	bHasRoughnessMap = false;
	bHasDisplacementMap = false;
	bHasAmbientOcclusionMap = false;
	bHasBlendMap = false;
	bHasTexture = false;
}


Material::Material(Gum::Filesystem::File materialfile)
    : Material()
{
    if(!Gum::Filesystem::fileExists(materialfile))
    {
        Gum::Output::error("Material: cannot read file: " + materialfile.toString() + " is not a file!");
        return;
    }

    std::vector<unsigned char> bytes;
    Gum::Codecs::unzip(materialfile, [&bytes](const char* data, const unsigned int len) {
        for(unsigned int i = 0; i < len; i++)
            bytes.push_back(data[i]);
    });

    SerializationData ndata(bytes.data(), bytes.size());
    ndata >> *this;
}

Material::~Material()
{

}


void Material::saveToFile(const Gum::Filesystem::File& file, const unsigned int& filetype)
{
    if(file.getType() != Gum::Filesystem::Filetype::FILE)
    {
        Gum::Output::error("Material: cannot save to file: " + file.toString() + " is not a file!");
        return;
    }

    std::stringstream stream;

    SerializationData data;
    data << *this;
    unsigned int len = 0;
    unsigned char* bytes = data.getData(len);
    for(unsigned int i = 0; i < len; i++)
        stream << bytes[i];

    Gum::Codecs::zip(file, 7, stream);
}


//
//Getter
//
bool Material::isReflective() 								    { return this->fReflectivity > 0.0f; }
bool Material::isRefractive() 								    { return this->fRefractivity > 0.0f; }
bool Material::isTransparent() 								    { return this->bTransparency; }
bool Material::hasFlippedNormals()							  { return this->bFlipNormal; }
float& Material::getSpecularity() 							  { return this->fSpecularity; }
float& Material::getRoughness() 							    { return this->fRoughness; }
float& Material::getReflectivity()							  { return this->fReflectivity; }
float& Material::getRefractivity()							  { return this->fRefractivity; }
int Material::getTextureMultiplier()						  { return this->iTextureMultiplier; }
std::string Material::getName() 							    { return this->sName; }
color Material::getColor()								        { return this->vColor; }
Texture* Material::getTexture(unsigned int index) { return this->mTextures[index]; }
int Material::numTextures() 								      { return this->iNumUsableTextures; }


//
//Setter
//
void Material::setName(std::string name) 					      { this->sName = name; }
void Material::setColor(color col)					            { this->vColor = col; }
void Material::setSpecularity(float specularity)			  { this->fSpecularity = specularity; }
void Material::setRoughness(float roughness)				    { this->fRoughness = roughness; }
void Material::setReflectivity(float reflectivity)			{ this->fReflectivity = reflectivity; }
void Material::setRefractivity(float refractivity)			{ this->fRefractivity = refractivity; }
void Material::setTextureMultiplier(float texMultiplier){ this->iTextureMultiplier = texMultiplier; }
void Material::setIsTransparency(bool isTransparent)		{ this->bTransparency = isTransparent; }
void Material::flipNormals(bool shouldFlip)					    { this->bFlipNormal = shouldFlip; }





/*
	Texture unit 16 is always the ShadowMap called "ShadowMap"
	Texture unit 15 is always the EnviormentMap called "Enviorment"
	Texture unit 14 is always the NormalMap called "normalmap"
	Texture unit 13 is always the DisplacementMap called "Displacement"
	Texture unit 12 is always the BlendMap called "blendtex"
	Texture unit 11 is always the Reflection Texture called "reflectionmap"
	Texture unit 10 is always the Refraction Texture called "refractionmap"
	Texture unit 9 is always the SpecularMap Texture called "specularmap"
	Texture unit 8 is always the roughness Texture called "roughnessmap"
	Texture unit 7 is always the ambient occlusion Texture called "ambientOcclusionmap"
	
*/
void Material::setTexture(Texture *tex, unsigned int index)
{
    if(tex == nullptr)
    {
		    delTexture(index);
        return;
    }

	  mTextures[index] = tex;

    switch(index)
    {
      case GUM_MATERIAL_NORMAL_MAP:             this->bHasNormalMap = true;           break;
      case GUM_MATERIAL_DISPLACEMENT_MAP:       this->bHasDisplacementMap = true;     break;
      case GUM_MATERIAL_BLEND_MAP: 	            this->bHasBlendMap = true;            break;
      case GUM_MATERIAL_REFLECTION_MAP: 	      this->bHasReflectionMap = true;       break;
      case GUM_MATERIAL_REFRACTION_MAP: 	      this->bHasRefractionMap = true;	      break;
      case GUM_MATERIAL_SPECULAR_MAP: 	        this->bHasSpecularMap = true;         break;
      case GUM_MATERIAL_ROUGHNESS_MAP:	        this->bHasRoughnessMap = true;        break;
      case GUM_MATERIAL_AMBIENT_OCCLUSION_MAP:  this->bHasAmbientOcclusionMap = true; break;
      default:                                  this->bHasTexture = true;             break;
    }
}

void Material::delTexture(unsigned int index)
{
  if(Tools::mapHasKey(mTextures, index))
	  mTextures.erase(mTextures.find(index));

    switch(index)
    {
      case GUM_MATERIAL_NORMAL_MAP:             this->bHasNormalMap = false;           break;
      case GUM_MATERIAL_DISPLACEMENT_MAP:       this->bHasDisplacementMap = false;     break;
      case GUM_MATERIAL_BLEND_MAP: 	            this->bHasBlendMap = false;            break;
      case GUM_MATERIAL_REFLECTION_MAP: 	      this->bHasReflectionMap = false;       break;
      case GUM_MATERIAL_REFRACTION_MAP: 	      this->bHasRefractionMap = false;	      break;
      case GUM_MATERIAL_SPECULAR_MAP: 	        this->bHasSpecularMap = false;         break;
      case GUM_MATERIAL_ROUGHNESS_MAP:	        this->bHasRoughnessMap = false;        break;
      case GUM_MATERIAL_AMBIENT_OCCLUSION_MAP:  this->bHasAmbientOcclusionMap = false; break;
      default:                                  this->bHasTexture = false;             break;
    }
}

void Material::bindTextures()
{
	for (auto it : mTextures)
        it.second->bind(it.first);
}
void Material::unbindTextures()
{
	for (auto it : mTextures)
        it.second->unbind(it.first);
}

bool Material::hasNormalMap() 			    { return this->bHasNormalMap; }
bool Material::hasSpecularMap() 		    { return this->bHasSpecularMap; }
bool Material::hasReflectionMap() 		  { return this->bHasReflectionMap; }
bool Material::hasRefractionMap() 		  { return this->bHasRefractionMap; }
bool Material::hasRoughnessMap() 		    { return this->bHasRoughnessMap; }
bool Material::hasDisplacementMap() 	  { return this->bHasDisplacementMap; }
bool Material::hasAmbientOcclusionMap() { return this->bHasAmbientOcclusionMap; }
bool Material::hasBlendMap() 			      { return this->bHasBlendMap; }
bool Material::hasTexture() 			      { return this->bHasTexture; }

void Material::onDeserialize()
{

}

SerializationData& Material::serialize(SerializationData& data)
{
    data & sName & fReflectivity & fRefractivity & fSpecularity & fRoughness & iTextureMultiplier & iNumUsableTextures & bTransparency & bFlipNormal;

    int numtex = mTextures.size();
    data & numtex;
    
    if(data.isSavingData())
    {
        for(auto it : mTextures)
        {
            int type = it.second->getType();
            int uniform = it.first;
            data & uniform & type & *it.second;
        }
    }
    else
    {
        for(int i = 0; i < numtex; i++)
        {
            int type = 0;
            int uniform = 0;
            data & uniform & type;
            Texture* tex = nullptr;
            switch(static_cast<Texture::Type>(type))
            {
                case Texture::TEXTURE2D: tex = new Texture2D(); break;
                case Texture::TEXTURE3D:
                case Texture::TEXTURECUBE:
                case Texture::TEXTUREHDR:
                default: break;
            }
            data & *tex;
            setTexture(tex, uniform);
        }
    }

    return data;
    //return data & fRoughness & fSpecularity;
}