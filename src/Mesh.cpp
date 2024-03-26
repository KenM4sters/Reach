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
Mesh::Mesh(std::shared_ptr<VertexArray>& vertex_array, TransformProps* transform_props)
    : m_vertexArray(vertex_array), m_transformProps(transform_props)
{   
    
}