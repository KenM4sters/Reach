#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 vUv;
out vec3 frag_pos;
out vec3 normal;

void main() 
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    vUv = a_uv;
    frag_pos = vec3(model * vec4(a_pos, 1.0));
	normal = mat3(transpose(inverse(model))) * a_normal;
}

