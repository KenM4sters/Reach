#pragma once
#include "Core.h"

enum OBJECT_TYPE 
{
    MESH,
    LIGHT,
};

// Holds the transform properties of a Mesh.
struct TransformProps {
    glm::vec3 Translation   = glm::vec3(0.0f);
    glm::quat Rotation      = glm::quat(glm::vec3(0.0f));
    glm::mat4 ModelMatrix   = glm::mat4(1.0f);
};


class SceneObject {
    public:
        SceneObject(OBJECT_TYPE type, TransformProps* props)
            : m_objectType(type), m_transformProps(props) 
        {
            
        }
        virtual ~SceneObject() {}
    protected:
        OBJECT_TYPE m_objectType;
        TransformProps* m_transformProps;
        
};