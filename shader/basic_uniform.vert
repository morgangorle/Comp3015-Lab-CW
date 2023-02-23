#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec4 camPosition;
out vec3 camNorm;




//Matrixes
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ProjectionMatrix;


void getCamSpaceValues(out vec3 norm, out vec4 position){
    norm = normalize(NormalMatrix*VertexNormal);
    position = ModelViewMatrix * vec4(VertexPosition,1.0);
}

void main()
{
    getCamSpaceValues(camNorm,camPosition);
    gl_Position = MVP*vec4(VertexPosition,1.0);
}
