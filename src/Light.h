#pragma once
#include "SceneObject.h"
#include "Mesh.h"


class Light : public SceneObject {
    public:
        Light(OBJECT_TYPE type, TransformProps* props)
            : SceneObject(type, props) 
        {

        }
        ~Light() {}
    protected:
};