#include "Mesh.h"

//================================================================
// Material
//================================================================
Material::Material(std::shared_ptr<Shader>& shader) 
    : m_shader(shader)
{

}

//================================================================
// Mesh
//================================================================
Mesh::Mesh(std::shared_ptr<VertexArray>& vertex_array, Material* material, TransformProps* transform_props, OBJECT_TYPE type)
    : SceneObject(type), m_vertexArray(vertex_array), m_material(material), m_transformProps(transform_props)
{   
    
}