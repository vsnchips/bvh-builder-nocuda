#version 150 core
uniform vec4 gColor;

out vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

//Lighting Regular Constants
const vec3 lightDir = vec3(0.1, 0.1, -1);
const vec3 ambientColor = vec3(0.3, 0.3, 0.3);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);
const float shininess = 16.0;

uniform mat4 viewMat;
uniform mat4 projectionMat; //might not use.. 

void main() {
	// for picking-selection
	if ( !(gColor.r==255) || !(gColor.g==255) || !(gColor.b==255)  ) {
		color = vec4(gColor.r/255.0, gColor.g/255.0, gColor.b/255.0, 1.0);
		return;
	}

    vec3 normal = fragNormal;
	normal = cross(dFdx(fragPosition), dFdy(fragPosition)); // flat shading
	normal = normalize(normal);
    vec3 lightDir = normalize(-lightDir);

    float lambertian = max(dot(lightDir,normal), 0.0);
    //lambertian = 1;
    float specular = 0.0;

    if (lambertian > 0.0) {
        vec3 viewDir = normalize(-fragPosition);

        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.0);

        //specular = pow(specAngle, shininess);
    }

    vec3 fragColor = ambientColor +
        lambertian * diffuseColor +
        specular * specColor;

    color = vec4(fragColor, 1.0);
}
