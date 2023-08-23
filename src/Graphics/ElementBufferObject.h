#pragma once
#include <vector>

class ElementBufferObject
{
private:
    unsigned int ieboID = 0;
    unsigned int iLength = 0;

    void createNative();
    void destroyNative();

public:
    ElementBufferObject();
    ~ElementBufferObject();

    void bind();
    void unbind();


    void setData(const std::vector<unsigned int>& indices);

    unsigned int getID() const;
    unsigned int getLength() const;
};