#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Normal; //normal 
out vec4 Position; //position
out vec2 TexCoord; //Texture coordinates

out vec3 Vec; // Pos for Skybox

uniform int passNum;




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
    Vec = VertexPosition;

    TexCoord = VertexTexCoord;
    getCamSpaceValues(Normal,Position);

    if(passNum == 2)
    {
    // Handle Skybox specific stuff
        vec4 StandardOutput = MVP * vec4(VertexPosition,1.0);
        StandardOutput.z = 1.0f;
        gl_Position = StandardOutput.xyzw;

    }
    else
    {
    //Do standard stuff
    gl_Position = MVP * vec4(VertexPosition,1.0);

    }


}
