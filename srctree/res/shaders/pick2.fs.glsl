#version 150 core

out vec4 color;

in vec3 fragPosition;
in vec3 fragNormal;

uniform int id;

void main() {

    vec3 fragColor = vec3(0.1,0,0.1);
    fragColor.g = id;
    color = vec4(fragColor, 1.0);
}
