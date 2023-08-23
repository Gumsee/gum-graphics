#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject()  { createNative(); }
ElementBufferObject::~ElementBufferObject() { destroyNative(); }

unsigned int ElementBufferObject::getID() const     { return this->ieboID; }
unsigned int ElementBufferObject::getLength() const { return this->iLength; }