#include "Model.h"
#include "Context/OpenGL/OpenGLTexture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"


void Model::LoadModel(const std::string &path) {
    // We need to know the API that we've selected so that we can instantiate the correct
    // textures and meshes.
    m_selectedAPI = RendererAPI::GetAPI();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    m_dir = path.substr(0, path.find_last_of("/")); // Generates path from root dir to where the model is saved.

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessModelMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }

}   

Mesh Model::ProcessModelMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<std::shared_ptr<Texture2D>> textures;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 p_vector;
        p_vector.x = mesh->mVertices[i].x;
        p_vector.y = mesh->mVertices[i].y;
        p_vector.z = mesh->mVertices[i].z;
        vertex.Position = p_vector;

        if(mesh->HasNormals()) {
            p_vector.x = mesh->mNormals[i].x;
            p_vector.y = mesh->mNormals[i].y;
            p_vector.z = mesh->mNormals[i].z;
            vertex.Normal = p_vector;
        }
        if(mesh->mTextureCoords[0]) {
            glm::vec2 p;
            p.x = mesh->mTextureCoords[0][i].x;
            p.y = mesh->mTextureCoords[0][i].y;
            vertex.UV = p;

        } else {
            vertex.UV = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::shared_ptr<Texture2D>> diffuseMaps = LoadModelTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<std::shared_ptr<Texture2D>> specularMaps = LoadModelTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<std::shared_ptr<Texture2D>> normalMaps = LoadModelTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<std::shared_ptr<Texture2D>> heightMaps = LoadModelTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // Next step it to finally create our mesh from our vertices, indices, and materials.
    Material* mat = LoadMaterial(material, m_shader);
    VertexBuffer* vbo = VertexBuffer::Create(vertices, vertices.size());
    IndexBuffer* ebo = IndexBuffer::Create(indices.data(), indices.size());
    auto vao = static_cast<std::shared_ptr<VertexArray>>(VertexArray::Create(vbo, ebo));
    
    return Mesh(vao, mat, new TransformProps(), OBJECT_TYPE::MODEL);
}

std::vector<std::shared_ptr<Texture2D>> Model::LoadModelTextures(aiMaterial* mat, aiTextureType type, std::string type_name) {
    std::vector<std::shared_ptr<Texture2D>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        auto str_c = str.C_Str();
        if(!m_textures_loaded.count(str_c)) {
            std::shared_ptr<Texture2D> texture;
            switch(m_selectedAPI) 
            {
                case API::VOID:
                    throw std::runtime_error("ERROR::VertexBuffer::Create() - Renderer::m_rendererAPI::API is currently set to VOID!");
                    break;
                case API::OPEN_GL:
                    texture = std::make_shared<OpenGLTexture2D>(OpenGLTexture2D(str_c, "", m_dir));
                    break;
                case API::VULKAN:
                    throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
                    break;
            }
            textures.push_back(texture);
            m_textures_loaded[str_c] = texture;
        } else {
            textures.push_back(m_textures_loaded[str_c]);
        }
    }
    return textures;
}


Material* Model::LoadMaterial(aiMaterial* mat, std::shared_ptr<Shader>& shader) {
    auto material = new Material(shader);
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material->GetProps()->Diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material->GetProps()->Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material->GetProps()->Specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material->GetProps()->Shininess = shininess;

    return material;
}