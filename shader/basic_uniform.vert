#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Normal; //normal 
out vec4 Position; //position




//Matrixes
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ProjectionMatrix;

void getCamSpaceValues (out vec3 norm, out vec4 position){
    norm = normalize(NormalMatrix * VertexNormal);
    position = (ModelViewMatrix * vec4(VertexPosition,1.0));
}

void main()
{
    getCamSpaceValues(Normal,Position);

    gl_Position = MVP * vec4(VertexPosition,1.0);


}
