#include "Object3D.h"
#include "ElementBufferObject.h"
#include "System/Output.h"
#include "Variables.h"
#include <Essentials/Tools.h>
#include <System/MemoryManagement.h>
#include <Codecs/Scene3DLoader.h>
#include <Codecs/Zip.h>
#include <string>


Object3D::RenderFunc Object3D::pRenderStripFunc = [](Object3D* obj) {
    obj->pVertexArrayObject->render((unsigned int)obj->vInstances.size());
};
Object3D::RenderFunc Object3D::pRenderTessellatedStripFunc = [](Object3D* obj) {
    obj->pVertexArrayObject->renderTesselated((unsigned int)obj->vInstances.size());
};
Object3D::RenderFunc Object3D::pRenderIndexedFunc = [](Object3D* obj) {
    obj->pVertexArrayObject->renderIndexed((unsigned int)obj->vInstances.size());
};
Object3D::RenderFunc Object3D::pRenderTessellatedIndexedFunc = [](Object3D* obj) {
    obj->pVertexArrayObject->renderTesselatedIndexed((unsigned int)obj->vInstances.size());
};

Object3D::Object3D(bool initvao)
{
	this->pShader = nullptr;
    this->pIndexBuffer = nullptr;

    if(!Gum::Graphics::RUN_HEADLESS && initvao)
    {
        pVertexArrayObject = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLES);
        pVertexArrayObject->onRenderCountUpdate([this]() {
            selectRenderFunc();
        });
    }
}

/** Loads the Object files content into memory and loads
 *  @param[in] modelFilePath Full path to object file
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object3D::Object3D(const Gum::File& modelFile, const std::string& name) : Object3D(true)
{
    if(modelFile.getType() != Gum::Filesystem::Filetype::FILE)
    {
        Gum::Output::error("Object3D: cannot read file: " + modelFile.toString() + " is not a file!");
        return;
    }

    std::string fileExtension = "";
    std::vector<std::string> splitstr = Tools::splitStr(modelFile.toString(), '.');
    if(splitstr.size() > 1)
        fileExtension = splitstr[splitstr.size()-1];

	//Create and add Properties
    sName = name;
    if(Tools::mapHasKey(Mesh::mLoadedMeshes, modelFile.toString()))
    {
        pMesh = Mesh::mLoadedMeshes[modelFile.toString()];
    }
    else if(fileExtension == "gumobj")
    {
        pMesh = new Mesh(modelFile.getName());
        std::vector<unsigned char> bytes;
        Gum::Codecs::unzip(modelFile, [&bytes](const char* data, const unsigned int len) {
            for(unsigned int i = 0; i < len; i++)
                bytes.push_back(data[i]);
        });
        
        SerializationData ndata(bytes.data(), bytes.size());
        ndata >> *this;
        Mesh::mLoadedMeshes[modelFile.toString()] = pMesh;
    }
    else
    {
        pMesh = new Mesh(modelFile.getName());
        
        Scene3DLoader loader;
        loader.iterateMeshes([this]([[maybe_unused]]unsigned int currentMesh, [[maybe_unused]]unsigned int numMeshes, Mesh* mesh, [[maybe_unused]]Bone* rootbone, [[maybe_unused]]std::vector<Bone*> bones) {
            pMesh->addMesh(mesh);
            Gum::_delete(mesh);
        });
        loader.load(modelFile);
        Mesh::mLoadedMeshes[modelFile.toString()] = pMesh;
    }

    load();
}


/** Create an object according to a given mesh
 *  @param[in] mesh Mesh containing all necessary information
 *  @param[in] Shader Shader used to render object
 *  @param[in] name Identifier, also used in the objectmanager in most cases
 */
Object3D::Object3D(Mesh *mesh, std::string name) : Object3D(true)
{
	//Create and add Properties
	sName = name;
	pMesh = mesh;

  load();
}

Object3D::~Object3D()
{
    Gum::_delete(pIndexBuffer);
	Gum::_delete(pVertexArrayObject);
	Gum::_delete(pTransMatricesVBO);
	Gum::_delete(pIndividualColorsVBO);
	Gum::_delete(pVertexVBO);
	for(Object3DInstance *inst : vInstances)
		Gum::_delete(inst);
}

/** Gernerates the Objects Vertex Array and Buffers
 *  It also creates the bounding box
 *  and calculates th furthest away point from the objects origin
 */ 
void Object3D::load()
{
    if(Gum::Graphics::RUN_HEADLESS)
      return;

    if(pVertexArrayObject == nullptr)
    {
        pVertexArrayObject = new VertexArrayObject(VertexArrayObject::PrimitiveTypes::TRIANGLES);
        pVertexArrayObject->onRenderCountUpdate([this]() {
            selectRenderFunc();
        });
    }

	if(pMesh != nullptr)
	{
        //Binding
      pVertexArrayObject->bind();
      pVertexVBO = new VertexBufferObject<Vertex>();
      pVertexVBO->setData(pMesh->getVertexBuffer(), Gum::Graphics::DataState::STATIC);
    
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Winvalid-offsetof"
      pVertexArrayObject->addAttribute(pVertexVBO,  0, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, position.x));
      pVertexArrayObject->addAttribute(pVertexVBO,  1, 2, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, textureCoord.x));
      pVertexArrayObject->addAttribute(pVertexVBO,  2, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, normal.x));
      pVertexArrayObject->addAttribute(pVertexVBO,  7, 3, Gum::Graphics::Datatypes::FLOAT, sizeof(Vertex), offsetof(Vertex, tangent.x));
      #pragma GCC diagnostic pop
        
      pTransMatricesVBO = new VertexBufferObject<mat4>();
      //pTransMatricesVBO->setData(vTransforms, GL_STREAM_DRAW);
      pVertexArrayObject->addAttributeMat4(pTransMatricesVBO, 3, Gum::Graphics::Datatypes::FLOAT, 1);

      pIndividualColorsVBO = new VertexBufferObject<vec4>();
      pVertexArrayObject->addAttribute(pIndividualColorsVBO, 10, 4, Gum::Graphics::Datatypes::FLOAT, sizeof(vec4), 0, 1);

      pVertexArrayObject->setVertexCount(pVertexVBO->getLength());

      if(pMesh->getIndexBuffer().size() > 0)
      {
          pIndexBuffer = new ElementBufferObject();
          pIndexBuffer->setData(pMesh->getIndexBuffer());
          pVertexArrayObject->addElementBuffer(pIndexBuffer);
      }
      pVertexArrayObject->unbind();

      selectRenderFunc();
	}
	else
	{
		Gum::Output::warn("ObjectCreation: Mesh is nullptr!");
	}
}


//Rendering stuff
void Object3D::render()
{
    renderMesh();
}

void Object3D::renderID()
{
    renderMesh();
}

void Object3D::renderMesh()
{
    pVertexArrayObject->bind();
    pRenderFunc(this);
    pVertexArrayObject->unbind();
}

Object3DInstance* Object3D::addInstance(Object3DInstance* instance)
{
	vInstances.push_back(instance);
	vTransforms.push_back(instance->getMatrix());
	pTransMatricesVBO->setData(vTransforms, Gum::Graphics::DataState::DYNAMIC);
    
	vIndividualColors.push_back(instance->getIndividualColor().getGLColor());
	pIndividualColorsVBO->setData(vIndividualColors, Gum::Graphics::DataState::STATIC);

    if(pAddInstanceCallback != nullptr)
        pAddInstanceCallback(instance);

    return instance;
}
Object3DInstance* Object3D::addInstance()
{
    return addInstance(new Object3DInstance(this));
}

Object3DInstance* Object3D::operator++() { return addInstance(); }


void Object3D::applyTransformationMatrix(Object3DInstance *inst)
{
    for(size_t i = 0; i < vInstances.size(); i++)
    {
        if(inst == vInstances[i])
        {
            //TODO
			      pTransMatricesVBO->setSingleData(inst->getMatrix(), (unsigned int)i); //To fix
            vTransforms[i] = inst->getMatrix(); // To get rid of
        }
    }

    pTransMatricesVBO->setData(vTransforms, Gum::Graphics::DataState::STATIC); //Make more efficient
}

void Object3D::selectRenderFunc()
{
    unsigned int type = pVertexArrayObject->getPrimitiveType();
    bool isStrip = type == VertexArrayObject::PrimitiveTypes::LINE_STRIP 
                || type == VertexArrayObject::PrimitiveTypes::LINE_LOOP 
                || type == VertexArrayObject::PrimitiveTypes::TRIANGLE_STRIP 
                || type == VertexArrayObject::PrimitiveTypes::QUAD_STRIP;

    if(isStrip)
    {
        if(bRenderTessellated)
            pRenderFunc = pRenderTessellatedStripFunc;
        else
            pRenderFunc = pRenderStripFunc;

        if(pMesh == nullptr)
            return;
    }
    else
    {
        if(bRenderTessellated)
            pRenderFunc = pRenderTessellatedIndexedFunc;
        else
            pRenderFunc = pRenderIndexedFunc;
    }
}

void Object3D::saveToFile(const Gum::Filesystem::File& file)
{
    if(file.getType() != Gum::Filesystem::Filetype::FILE)
    {
        Gum::Output::error("Object3D: cannot save to file: " + file.toString() + " is not a file!");
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
//Setter
//
void Object3D::setShaderProgram(ShaderProgram *shader)     { this->pShader = shader; }
void Object3D::setName(const std::string& name) 	         { this->sName = name; }
void Object3D::renderTessellated(bool tessellated)         { this->bRenderTessellated = tessellated; selectRenderFunc(); }
void Object3D::onAddInstance(AddInstanceCallback callback) { this->pAddInstanceCallback = callback; }


//
//Getter
//
Mesh*               Object3D::getMesh()                              { return pMesh; }
std::string 		Object3D::getName() 			                           { return sName; }
Object3DInstance* 	Object3D::getInstance(const unsigned int& index) { return vInstances[(size_t)index]; }
ShaderProgram*		Object3D::getShaderProgram()	                     { return pShader; }
unsigned int        Object3D::numInstances() 	                       { return (unsigned int)vInstances.size(); }
VertexArrayObject*  Object3D::getVertexArrayObject()                 { return pVertexArrayObject; }


SerializationData& Object3D::serialize(SerializationData& data)
{
    if(pMesh == nullptr)
        return data;

    return data & *pMesh;
}