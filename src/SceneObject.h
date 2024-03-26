#pragma once
#include "Core.h"

enum OBJECT_TYPE 
{
    MESH,
    LIGHT,
};

class SceneObject {
    public:
        SceneObject() {}
        virtual ~SceneObject() {}
    protected:
        OBJECT_TYPE m_objectType;
        
};