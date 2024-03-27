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
uniform vec3 CameraPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


void main() 
{
    vec3 ambient_shading = texture(texture_diffuse1, vUv).rgb * light.AmbientColor * light.Intensity;
    // Diffuse
    vec3 light_dir = normalize(light.Position - frag_pos);
    vec3 normal_dir = normalize(normal);
    float diffuse_angle = max(dot(normal_dir, light_dir), 0.0);
    vec3 diffuse_shading = texture(texture_diffuse1, vUv).rgb * diffuse_angle;
    // Specular
    vec3 view_dir = normalize(CameraPos - frag_pos);
    vec3 crit_reflection = reflect(-light_dir, normal_dir);
    float specular_intensity = pow(max(dot(view_dir, crit_reflection), 0.0), 32.0);
    vec3 specular_shading = texture(texture_specular1, vUv).rgb * specular_intensity;

    vec3 result = diffuse_shading;
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);



}