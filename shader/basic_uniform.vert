#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

//Light info
uniform vec4 LightPosition;
uniform vec3 Ld; //Diffuse
uniform vec3 La; //Ambient
uniform vec3 Ls; //Specular


// Material info
uniform vec3 Kd; //Diffuse
uniform vec3 Ka; //Ambient
uniform vec3 Ks; //Specular
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    vec3 n = normalize(NormalMatrix*VertexNormal);
    vec4 pos = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(LightPosition-pos));
    LightIntensity = Kd * Ld * max(dot(s,n),0.0);
    gl_Position = MVP*vec4(VertexPosition,1.0);
}
