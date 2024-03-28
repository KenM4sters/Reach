#pragma once
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Core.h"
#include "Mesh.h"
#include "Renderer/RendererAPI.h"

class Model : public SceneObject {
    public:
        Model(const std::string &path, std::shared_ptr<Shader>& shader, OBJECT_TYPE type, TransformProps* transform_props) 
            : SceneObject(type, transform_props), m_shader(shader) 
        {
            m_name = shader->GetName().substr(0, shader->GetName().find_first_of("_")); // Removes "_shader" from name - looks ugly on the interface.
            LoadModel(path);
        }
        ~Model() { std::cout << "Model has been destroyed" << std::endl; }
        
        void LoadModel(const std::string& path);
        std::vector<std::shared_ptr<Texture2D>> LoadModelTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
        Material* LoadMaterial(aiMaterial* mat, std::shared_ptr<Shader>& shader);

        inline TransformProps* GetTransformProps() {return m_transformProps;}
        inline const std::vector<Mesh>* GetMeshes() const {return &m_meshes;}
        const std::string GetName() const {return m_name;}
    private:
        Mesh ProcessModelMesh(aiMesh *mesh, const aiScene *scene);
        void ProcessNode(aiNode *node, const aiScene *scene);
    private:
        std::shared_ptr<Shader> m_shader = nullptr;
        API m_selectedAPI = API::OPEN_GL;
        TransformProps* m_transforms = nullptr;
        std::string m_name;

        std::vector<Mesh> m_meshes;
        std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures_loaded;
        std::string m_dir = "";
        bool bGammeCorrection = false;
    
};