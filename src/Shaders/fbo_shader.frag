#version 330 core

out vec4 FragColor;
in vec2 vUv;
uniform sampler2D tex;

void main() {
    const float gamma = 1.2;
    vec3 hdr_color = texture(tex, vUv).rgb;
  
    // reinhard tone mapping
    vec3 mapped = hdr_color / (hdr_color + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    // FragColor = vec4(mapped, 1.0);
    FragColor = texture(tex, vUv);
}