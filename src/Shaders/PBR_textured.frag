#version 330 core

struct Material {
    vec3 Albedo;
    float Metallic;
    float Roughness;
    float AO;
};

struct Light {
    vec3 Position;
    vec3 AmbientColor;
    float Intensity;
};

out vec4 FragColor;
in vec2 vUv;
in vec3 vFragPos;
in vec3 vNormal;

uniform vec4 uColor;
uniform vec3 CameraPos;

uniform Light light;
uniform Material material;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;
uniform sampler2D texture_ao;

uniform samplerCube convoluted_map;
uniform samplerCube prefiltered_map;
uniform sampler2D BRDF_map;

const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness); 


void main() 
{
    // Setting values as single float values of textures.
    vec3 Albedo      = pow(texture(texture_diffuse, vUv).rgb, vec3(2.2));
    vec3 Normal      = texture(texture_normal, vUv).rgb;
    vec3 Height      = texture(texture_height, vUv).rgb;
    float Metallic   = texture(texture_metallic, vUv).r;
    float Roughness  = texture(texture_roughness, vUv).r;
    float AO         = texture(texture_ao, vUv).r;

    // Setup
    //================================================================
    vec3 V = normalize(CameraPos - vFragPos);
    vec3 N = normalize(Normal); 
    vec3 R = reflect(-V, N);   

    vec3 L = normalize(light.Position - vFragPos);
    vec3 H = normalize(V + L);

    float Distance = length(light.Position - vFragPos);
    float attentuation = Distance / pow(Distance, 2);
    vec3 radiance = light.AmbientColor * attentuation * light.Intensity;
    // //================================================================
    
    // // BRDF
    // //================================================================
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, Roughness);
    float G = GeometrySmith(N, H, L, Roughness);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, V), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;  

    vec3 Ks = F;
    vec3 Kd = vec3(1.0) - F;
    Kd *= 1.0 - Metallic;

    float NdotL = max(dot(N, L), 0.0);        
    vec3 Lo = (Kd * Albedo / PI + specular) * radiance * NdotL;

    vec3 FR = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.Roughness);

    vec3 kS = FR;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;	  
    
    vec3 irradiance = texture(convoluted_map, N).rgb;
    vec3 diffuse    = irradiance * Albedo;
    
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefiltered_map, R,  Roughness * MAX_REFLECTION_LOD).rgb;   
    vec2 envBRDF  = texture(BRDF_map, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 final_specular = prefilteredColor * (FR * envBRDF.x + envBRDF.y);
    
    vec3 ambient = (kD * diffuse + final_specular) * AO; 
    vec3 color = ambient + Lo;

    // HDRI
    color = color/(color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);

}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}