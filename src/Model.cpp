#include "Model.h"
#include "model.h"


void Model::LoadModel(const std::string &path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    dir_ = path.substr(0, path.find_last_of("/")); // Generates path from root dir to where the model is saved.

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(ProcessModelMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }

}   

Mesh Model::ProcessModelMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;
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
    std::vector<Texture2D> diffuseMaps = LoadModelTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture2D> specularMaps = LoadModelTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture2D> normalMaps = LoadModelTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture2D> heightMaps = LoadModelTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // Set the material to the material properties of each mesh (useful when no textures have been set).

    Material* mat = LoadMaterial(material, shader_);

    return Mesh(geo, shader_, textures, mat);
}

std::vector<Texture2D> Model::LoadModelTextures(aiMaterial* mat, aiTextureType type, std::string type_name) {
    std::vector<Texture2D> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        auto str_c = str.C_Str();
        if(!textures_loaded_.count(str_c)) {
            Texture2D texture(SANDBOX_OBJECT, type_name);
            texture.LoadFromFile(str_c, dir_);
            textures.push_back(texture);
            textures_loaded_[str_c] = texture;
        } else {
            textures.push_back(textures_loaded_[str_c]);
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