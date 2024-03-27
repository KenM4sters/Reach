#pragma once
#include "SceneObject.h"
#include "Mesh.h"

struct PointLightProps {
    glm::vec3 AmbientColor{1.0f, 1.0f, 1.0f};
    float Intensity{1.0f};
};


class Light : public SceneObject {
    public:
        Light(OBJECT_TYPE type, TransformProps* transform_props, PointLightProps* light_props)
            : SceneObject(type, transform_props), m_lightProps(light_props) 
        {}
        ~Light() {}

        virtual inline PointLightProps* GetLightProps() { return m_lightProps;}
        virtual void SetLightProps(PointLightProps* light_props) {m_lightProps = light_props;}

    protected:
        PointLightProps* m_lightProps;
};

class PointLight : public Light {
    public:
        PointLight(OBJECT_TYPE type, TransformProps* transform_props, PointLightProps* light_props)
            : Light(type, transform_props, light_props)
        {}
        ~PointLight() {
            delete m_lightProps;
        }
    public:
        uint32_t test;
};