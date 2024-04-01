#pragma once
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Core.h"
#include "Mesh.h"
#include "Renderer/RendererAPI.h"

class Model : public SceneObject {
    public:
        Model(const std::string &path, OBJECT_TYPE type, Material* mat) 
            : SceneObject(type), m_material(mat) 
        {
            auto shader_name = m_material->GetShader()->GetName();   
            m_name = shader_name.substr(0, shader_name.find_first_of("_")); // Removes "_shader" from name - looks ugly on the interface.
            LoadModel(path);
        }
        ~Model() { std::cout << "Model has been destroyed" << std::endl; }
        
        void LoadModel(const std::string& path);

        // Getters
        inline TransformProps* GetTransformProps() {return m_transformProps;}
        inline const std::vector<Mesh>* GetMeshes() const {return &m_meshes;}
        inline const std::string GetName() const {return m_name;}
        inline Material* GetMaterial() {return m_material;}

    private:
        Mesh ProcessModelMesh(aiMesh *mesh, const aiScene *scene);
        void ProcessNode(aiNode *node, const aiScene *scene);
        std::vector<std::shared_ptr<Texture2D>> LoadModelTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
        Material* LoadMaterial(aiMaterial* mat, std::shared_ptr<Shader> shader);
    private:
        Material* m_material = nullptr;
        TransformProps* m_transforms = nullptr;
        std::string m_name = "";

        API m_selectedAPI = API::OPEN_GL;
        std::vector<Mesh> m_meshes;
        std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures_loaded;
        std::string m_dir = "";
        bool bGammeCorrection = false;
    
};