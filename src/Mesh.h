#pragma once
#include "Core.h"
#include "SceneObject.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

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

// Properties for PBR (Physically Based Rendering).
struct MaterialProps {
    glm::vec3 Albedo    = {0.8f, 0.8f, 0.8f};
    float Metalness     = 0.2f;
    float Roughness     = 0.8f;
    float AO            = 0.2f;
    std::vector<std::shared_ptr<Texture2D>> Textures;
};


class Material {
    public:
        Material(std::shared_ptr<Shader>& shader);
        ~Material()
        { 
        }
        
        // Getters
        inline std::shared_ptr<Shader> GetShader() { return m_shader;}
        inline MaterialProps* GetProps() { return &m_props; }

        // Setters
        void SetShader(std::shared_ptr<Shader>& shader) { m_shader = shader; }
        void SetProps(MaterialProps* props) { m_props = *props; }

    private:
        std::shared_ptr<Shader> m_shader = nullptr;
        MaterialProps m_props;

};

class Mesh : SceneObject {
    public:
        Mesh(std::shared_ptr<VertexArray>& vertex_aray, Material* material, OBJECT_TYPE type = OBJECT_TYPE::MESH);

        // Getters
        inline std::shared_ptr<VertexArray> GetVAO() const { return m_vertexArray; }
        inline Material* GetMaterial() const { return m_material; }
        inline TransformProps* GetTransformProps() const { return m_transformProps; }

        // Setters
        void SetVAO(std::shared_ptr<VertexArray>& vertex_array) { m_vertexArray = vertex_array; }
        void SetMaterial(Material* material) { m_material = material; }
        void SetTransformProps(TransformProps* transform_props) { m_transformProps = transform_props; }
        
    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        Material* m_material;
};