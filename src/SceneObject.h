#pragma once
#include "Core.h"

enum OBJECT_TYPE 
{
    MESH,
    LIGHT,
};

class SceneObject {
    public:
        SceneObject(OBJECT_TYPE type = OBJECT_TYPE::MESH)
            : m_objectType(type) 
        {
            
        }
        virtual ~SceneObject() {}
    protected:
        OBJECT_TYPE m_objectType;
        
};