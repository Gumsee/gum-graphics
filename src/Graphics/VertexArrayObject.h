#pragma once
#include <algorithm>
#include <functional>
#include <gum-maths.h>
#include <System/Output.h>
#include "ElementBufferObject.h"
#include "VertexBufferObject.h"

class VertexArrayObject
{
public:
  struct PrimitiveTypes
  {
    static const unsigned int POINTS;
    static const unsigned int TRIANGLES;
    static const unsigned int TRIANGLE_STRIP;
    static const unsigned int TRIANGLE_FAN;
    static const unsigned int LINES;
    static const unsigned int LINE_STRIP;
    static const unsigned int LINE_LOOP;
    static const unsigned int LINE_ADJACENCY;
    static const unsigned int QUADS;
    static const unsigned int QUAD_STRIP;
  };

private:
  struct AttributeProperties
  {
    const unsigned int index;
    const unsigned int vbo;
    const unsigned int dimension;
    const unsigned int type;
    const unsigned int divisor;
    const size_t stride;
    const size_t offset;
  };

  std::unordered_map<void*, unsigned int> iID;
  unsigned int iVertexCount;
  unsigned int iRenderCount;
  unsigned int iPrimitiveType;
  std::vector<AttributeProperties> vAttributes;
  unsigned int iIndexBuffer;
  std::function<void()> pOnRenderCountFunc;

  void updateRenderCount();
  void createNative();
  void destroyNative();
  void addAttributeNative(AttributeProperties& properties);

public:
    VertexArrayObject(const unsigned int& primitivetype);
    ~VertexArrayObject();

    void reinitializeAttributesAndID();

    void bind();
    void unbind();

    void render(const unsigned int& instances);
    void renderIndexed(const unsigned int& instances);
    void renderTesselated(const unsigned int& instances);
    void renderTesselatedIndexed(const unsigned int& instances);

    unsigned int addElementBuffer(ElementBufferObject* elembuffer);

    template<typename T>
    unsigned int addAttribute(VertexBufferObject<T>* vbo, const unsigned int& index, const unsigned int& dimension, 
        const unsigned int& type, const size_t& stride = sizeof(T), const size_t& offset = 0, const unsigned int& divisor = 0)
    {
        bind();
        vbo->bind();
        if(std::find_if(vAttributes.begin(), vAttributes.end(), [index](AttributeProperties prop){ return prop.index == index; }) != vAttributes.end())
          Gum::Output::warn("VertexArrayObject: Attribute " + std::to_string(index) + " has already been added! (Not doing anything..)");
        else
        {
          AttributeProperties props({
            .index = index, .vbo = vbo->getID(), .dimension = dimension, 
            .type = type, .divisor = divisor, .stride = stride, .offset = offset
          });
          addAttributeNative(props);
          vAttributes.push_back(props);
        }
        vbo->unbind();
        unbind();
        return index;
    }


    template<typename T>
    unsigned int addAttributeMat(VertexBufferObject<T>* vbo, unsigned int index, const unsigned int& dimension, unsigned int type, const unsigned int& divisor)
    {
        bind();
        vbo->bind();
        size_t rowSize = sizeof(float) * dimension;
        for(unsigned int i = 0; i < dimension; i++)
        {
          AttributeProperties props({
            .index = index + i, .vbo = vbo->getID(), .dimension = dimension, 
            .type = type, .divisor = divisor, .stride = dimension * rowSize, .offset = i * rowSize
          });
          addAttributeNative(props);
          vAttributes.push_back(props);
        }

        vbo->unbind();
        unbind();
        return index;
    }

    //Setter
	  void setPrimitiveType(const unsigned int& type);
    void setVertexCount(const unsigned int& count);
    void onRenderCountUpdate(std::function<void()> func);

    //Getter
    unsigned int numVertices() const;
    unsigned int getRenderCount() const;
    unsigned int getID(void* context) const;
    unsigned int getPrimitiveType() const;
};