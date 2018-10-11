#version 150 core

out vec4 color;

in vec3 fragPosition;
in vec3 fragNormal;

const vec3 lightDir = vec3(0., 0.25, -1);

const vec3 ambientColor = vec3(0.05, 0.35, 0.1);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);

const float shininess = 16.0;

uniform vec3 ucol;

void main() {

    vec3 diffuseColor = ucol;
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(-lightDir);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;

    if (lambertian > 0.0) {
        vec3 viewDir = normalize(-fragPosition);

        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.0);

        specular = pow(specAngle, shininess);
    }

    vec3 fragColor = ambientColor +
        lambertian * diffuseColor +
        specular * specColor;

//    fragColor = vec3(1.0);
    color = vec4(fragColor, 1.0);
}
