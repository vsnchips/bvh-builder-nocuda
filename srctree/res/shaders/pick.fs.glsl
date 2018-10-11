#version 150 core

out vec4 color;

in vec3 fragPosition;
in vec3 fragNormal;

const vec3 lightDir = vec3(0.25, 0.25, -1);

const vec3 ambientColor = vec3(0.05, 0.05, 0.1);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);

const float shininess = 16.0;

uniform vec3 ucol;

uniform int id;

void main() {

    vec3 fragColor = vec3(0);
    
    fragColor.r = float(1)/255.0;
    fragColor.r *= id;

  //  fragColor.g=1.0;

    color = vec4(fragColor, 1.0);
}
