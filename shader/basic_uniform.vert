#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

//Light info
uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 Ld; //Diffuse
    vec3 Ls; //Specular
    } Light;


// Material info
uniform struct MaterialInfo{
    vec3 Ka; //Ambient
    vec3 Kd; //Diffuse
    vec3 Ks; //Specular
    float Shininess; //Shininess factor

} Material;


//Matrixes
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
