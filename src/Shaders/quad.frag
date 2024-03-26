#version 330 core

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float shininess;
    sampler2D test;
};

out vec4 FragColor;
in vec2 vUv;
uniform vec4 uColor;
uniform Material material;

void main() 
{
    FragColor = vec4(texture(material.test, vUv).rgb * material.Ambient, 1.0);
}