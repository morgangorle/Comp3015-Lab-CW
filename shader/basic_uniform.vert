#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
//layout (location = 2) in vec2 VertexTexCoord;
//layout (location = 3) in vec4 VertexTangent;

out vec3 EyeNormal; //normal in eye coordinates
out vec4 EyePosition; //position in eye coordinates
out vec4 ProjTexCoord; //projected texture coordinates
/*
//Light info
uniform struct lightinfo
{
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
} Light;



void getCamSpaceValues(out vec3 norm, out vec4 position){
    norm = normalize(NormalMatrix*VertexNormal);
    position = ModelViewMatrix * vec4(VertexPosition,1.0);
}
*/

//Matrixes
uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ProjectorMatrix;
//uniform mat4 ProjectionMatrix;

void main()
{
    vec4 pos4 = vec4(VertexPosition, 1.0);

    EyeNormal = normalize(NormalMatrix * VertexNormal);
    EyePosition = ModelViewMatrix * pos4;

    ProjTexCoord = ProjectorMatrix * (ModelMatrix * pos4);

    gl_Position = MVP*pos4;


}
