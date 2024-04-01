#pragma once
#include "Core.h"

enum OBJECT_TYPE 
{
    MESH,
    LIGHT,
    MODEL
};

// Holds the transform properties of a Mesh.
struct TransformProps {
    glm::vec3 Translation   = glm::vec3(0.0f);
    glm::quat Rotation      = glm::quat(glm::vec3(0.0f));
    glm::vec3 Scale         = glm::vec3(1.0f);
    glm::mat4 ModelMatrix   = glm::mat4(1.0f);
};


class SceneObject {
    public:
        SceneObject(OBJECT_TYPE type)
            : m_objectType(type) 
        {
            m_transformProps = new TransformProps();
        }
        ~SceneObject() {}

        // Getters
        const inline OBJECT_TYPE& GetObjectType() const { return m_objectType; }
        inline TransformProps* GetTransformProps() { return m_transformProps; }

        // Setters
        void SetObjectType(OBJECT_TYPE type) { m_objectType = type; }
        void SetTransformProps(TransformProps* props) {m_transformProps = props;}
    protected:
        OBJECT_TYPE m_objectType;
        TransformProps* m_transformProps;
        
};