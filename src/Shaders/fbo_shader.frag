#version 330 core

out vec4 FragColor;
in vec2 vUv;
uniform sampler2D tex;

void main() 
{
    FragColor = texture(tex, vUv);
}