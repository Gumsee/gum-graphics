#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(const unsigned int& primitivetype)
{
    this->ivaoID = 0;
    this->iIndexBuffer = 0;
    this->iVertexCount = 0;
    this->iPrimitiveType = primitivetype;

    createNative();
}

VertexArrayObject::~VertexArrayObject() 
{
    destroyNative();
	vAttributes.clear();
}


unsigned int VertexArrayObject::addElementBuffer(ElementBufferObject* elembuffer)
{
    iRenderCount = elembuffer->getLength();
    iIndexBuffer = elembuffer->getID();
    updateRenderCount();
    return elembuffer->getID();
}

void VertexArrayObject::updateRenderCount()
{
    /*if(iIndexBuffer == 0)
    {
        int divider = (iPrimitiveType == PrimitiveTypes::LINES) ? 2 
                    : (iPrimitiveType == PrimitiveTypes::TRIANGLES) ? 3 
                    : (iPrimitiveType == PrimitiveTypes::QUADS) ? 4
                    : 1;
        iRenderCount = iVertexCount / divider;
    }*/
    if(pOnRenderCountFunc != nullptr)
        pOnRenderCountFunc();
}

//
// Setter
//
void VertexArrayObject::setVertexCount(const unsigned int& count)       { this->iVertexCount = count; this->iRenderCount = count; updateRenderCount(); }
void VertexArrayObject::setPrimitiveType(const unsigned int& type)      { this->iPrimitiveType = type; updateRenderCount(); }
void VertexArrayObject::onRenderCountUpdate(std::function<void()> func) { this->pOnRenderCountFunc = func; }

//
// Getter
//
unsigned int VertexArrayObject::numVertices() const      { return this->iVertexCount; }
unsigned int VertexArrayObject::getID() const            { return this->ivaoID; }
unsigned int VertexArrayObject::getRenderCount() const   { return this->iRenderCount; }
unsigned int VertexArrayObject::getPrimitiveType() const { return this->iPrimitiveType; }