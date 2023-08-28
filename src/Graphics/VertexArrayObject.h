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
    unsigned int ivaoID;
    unsigned int iIndexBuffer;
    unsigned int iVertexCount;
    unsigned int iRenderCount;
    unsigned int iPrimitiveType;
    std::vector<unsigned int> vBufferIDs;
	std::vector<unsigned int> vAttributes;
    std::function<void()> pOnRenderCountFunc;

    void updateRenderCount();
    void createNative();
    void destroyNative();
    void addAttributeNative(const unsigned int& index, const unsigned int& dimension, const unsigned int& type, const size_t& stride, const size_t& offset, const unsigned int& divisor);
    void addAttributeMat4Native(unsigned int index, unsigned int type, const unsigned int& divisor);
    void addAttributeMat3Native(unsigned int index, unsigned int type, const unsigned int& divisor);

public:
    VertexArrayObject(const unsigned int& primitivetype);
    ~VertexArrayObject();

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
        if(std::find(vAttributes.begin(), vAttributes.end(), index) != vAttributes.end())
            Gum::Output::warn("VertexArrayObject: Attribute " + std::to_string(index) + " has already been added! (Not doing anything..)");
        else
            addAttributeNative(index, dimension, type, stride, offset, divisor);
        vbo->unbind();
        unbind();
        return index;
    }


    template<typename T>
    unsigned int addAttributeMat4(VertexBufferObject<T>* vbo, unsigned int index, unsigned int type, const unsigned int& divisor)
    {
        bind();
        vbo->bind();
        addAttributeMat4Native(index, type, divisor);
        vbo->unbind();
        unbind();
        return index;
    }


    template<typename T>
    unsigned int addAttributeMat3(VertexBufferObject<T>* vbo, unsigned int index, unsigned int type, const unsigned int& divisor)
    {
        bind();
        vbo->bind();
        addAttributeMat3Native(index, type, divisor);
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
    unsigned int getID() const;
    unsigned int getPrimitiveType() const;
};