#pragma once
#include "../Core.h"

class Texture {
    public:
        Texture() {}
        ~Texture() {}
        virtual inline std::string& GetName() = 0;
        virtual void Bind() = 0;
        virtual void Bind(uint32_t& count) = 0;
        virtual void Unbind() = 0;
        virtual void Unbind(uint32_t& count) = 0;
    protected:
        std::string m_debugName;
};

class Texture2D : public Texture {
    public:
        static std::shared_ptr<Texture2D> Create(const char* path, const char* name);
        inline std::string& GetName() override { return m_debugName; }
        virtual inline uint32_t GetWidth() {return m_width;}
        virtual inline uint32_t GetHeight() {return m_height;}
        void Bind() override;
        void Bind(uint32_t& count) override;
        void Unbind() override;
        void Unbind(uint32_t& count) override;
    protected:        
        uint32_t m_ID;
        uint32_t m_width;
        uint32_t m_height;
};