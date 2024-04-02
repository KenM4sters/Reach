#pragma once
#include "SceneObject.h"
#include "Mesh.h"

struct PointLightProps {
    glm::vec3 AmbientColor{1.0f, 1.0f, 1.0f};
    float Intensity{30.0f};
};


class Light : public SceneObject {
    public:
        Light(OBJECT_TYPE type, PointLightProps* light_props)
            : SceneObject(type), m_lightProps(light_props) 
        {}
        ~Light() {}

        virtual inline PointLightProps* GetLightProps() { return m_lightProps;}
        virtual void SetLightProps(PointLightProps* light_props) {m_lightProps = light_props;}

    protected:
        PointLightProps* m_lightProps;
};

class PointLight : public Light {
    public:
        PointLight(OBJECT_TYPE type, PointLightProps* light_props)
            : Light(type, light_props)
        {}
        ~PointLight() {
            delete m_lightProps;
        }
};