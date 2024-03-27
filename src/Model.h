#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Core.h"
#include "Mesh.h"

class Model : public SceneObject {
    public:
        Model(const std::string &path, std::shared_ptr<Shader>& shader, OBJECT_TYPE type, TransformProps* transform_props) 
            : SceneObject(type, transform_props), shader_(shader) 
        {
            LoadModel(path);
        }
        ~Model() { std::cout << "model is destroyed" << std::endl; }
        void LoadModel(const std::string &path);
        void ProcessNode(aiNode *node, const aiScene *scene);

        std::vector<Texture2D> LoadModelTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
        Material* LoadMaterial(aiMaterial* mat, std::shared_ptr<Shader>& shader);
        Mesh ProcessModelMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Mesh> GetMeshes() const {
            return meshes_;
        }
    private:
        TransformProps* transforms_ = nullptr;
        std::vector<Mesh> meshes_;
        std::unordered_map<std::string, Texture2D> textures_loaded_;
        std::shared_ptr<Shader> shader_ = nullptr;
        std::string dir_ = "";
        bool bGammeCorrection = false;
};