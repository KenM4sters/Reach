#pragma once
#include "Core.h"
#include "SceneObject.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

/**
 * The mesh class is integral to our scene as it's essentially how we encapsulate VAOs together
 * with shaders to render objects to the screen. 
 * 
 * Shaders will also be abstracted further into a Material class since they're mostly for describing
 * how objects appear on the screen (although obviously they still affect the vertices themselves).
 * 
 * Since the way materials interact with lighting will change as we develop from more basic shading
 * models, such as Phong shading, to more complicated ones, such as PBR, I've also abstracted 
 * the properties of a material into a seperate Struct. 
*/

// Phong-Shading model for now - will eventualy be PBR.
struct MaterialProps {
    glm::vec3 Ambient   = {0.4f, 0.4f, 0.4f};
    glm::vec3 Diffuse   = {0.8f, 0.8f, 0.8f};
    glm::vec3 Specular  = {0.2f, 0.2f, 0.2f};
    float Shininess     = 8.0f;
};


class Material {
    public:
        Material(std::shared_ptr<Shader>& shader);
        ~Material()
        { 
            delete m_props; 
        }
        
        // Getters
        std::shared_ptr<Shader> GetShader() const { return m_shader;}
        MaterialProps* GetProps() const { return m_props; }

        // Setters
        void SetShader(std::shared_ptr<Shader>& shader) { m_shader = shader; }
        void SetMaterialProps(MaterialProps* props) { m_props = props; }

    private:
        std::shared_ptr<Shader> m_shader;
        MaterialProps* m_props;

};

// Holds the transform properties of a Mesh.
struct TransformProps {
    glm::vec3 Translation   = glm::vec3(0.0f);
    glm::quat Rotation      = glm::quat(glm::vec3(0.0f));
    glm::mat4 ModelMatrix   = glm::mat4(1.0f);
};

class Mesh : SceneObject {
    public:
        Mesh(std::shared_ptr<VertexArray>& vertex_aray, TransformProps* transform_props);
        ~Mesh() {}

        // Getters
        std::shared_ptr<VertexArray> GetVAO() const { return m_vertexArray; }
        std::shared_ptr<Material> GetMaterial() const { return m_material; }
        TransformProps* GetTransformProps() const { return m_transformProps; }

        // Setters
        void SetVAO(std::shared_ptr<VertexArray>& vertex_array) { m_vertexArray = vertex_array; }
        void SetMaterial(std::shared_ptr<Material>& material) { m_material = material; }
        void SetTransformProps(TransformProps* transform_props) { m_transformProps = transform_props; }
        
    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        std::shared_ptr<Material> m_material;
        TransformProps* m_transformProps;
};