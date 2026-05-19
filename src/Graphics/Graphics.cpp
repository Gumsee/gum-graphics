#include "Graphics.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Texture.h"

namespace Gum {
namespace Graphics
{
  void destroyAllAssets()
  {
    Material::destroyAllMaterials();
    Shader::destroyAllShaders();
    ShaderProgram::destroyAllShaders();
    Texture::cleanupAllLoadedTextures();
  }
}}