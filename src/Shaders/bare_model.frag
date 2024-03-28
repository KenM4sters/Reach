#version 330 core

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct Light {
    vec3 Position;
    vec3 AmbientColor;
    float Intensity;
};

out vec4 FragColor;
in vec2 vUv;
in vec3 frag_pos;
in vec3 normal;

uniform vec4 uColor;
uniform Light light;
uniform Material material;
uniform vec3 CameraPos;

void main() 
{
    vec3 ambient_shading = material.Ambient * light.AmbientColor * light.Intensity;
    // Diffuse
    vec3 light_dir = normalize(light.Position - frag_pos);
    vec3 normal_dir = normalize(normal);
    float diffuse_angle = max(dot(normal_dir, light_dir), 0.0);
    vec3 diffuse_shading = material.Diffuse * diffuse_angle;
    // Specular
    vec3 view_dir = normalize(CameraPos - frag_pos);
    vec3 crit_reflection = reflect(-light_dir, normal_dir);
    float specular_intensity = pow(max(dot(view_dir, crit_reflection), 0.0), 32.0);
    vec3 specular_shading = material.Specular * specular_intensity * material.Shininess;

    vec3 result = ambient_shading + diffuse_shading + specular_shading;
    FragColor = vec4(result, 1.0);
}