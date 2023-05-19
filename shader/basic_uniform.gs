#version 460

in vec3 Normal;
in vec4 Position;
in vec2 TexCoord;
in vec3 Vec;

out vec3 FragNormal;
out vec4 FragPosition;
out vec2 FragTexCoord;
out vec3 FragVec;

layout (points) in;

layout (triangle_strip,max_vertices = 4) out;

uniform float Size2;

uniform mat4 ProjectionMatrix;

out vec2 NewTexCoord;


void main()
{
	FragNormal = Normal;
	FragPosition = Position;
	FragTexCoord = TexCoord;
	FragVec = Vec;
	mat4 m = ProjectionMatrix;

	//setup vertex 1
	gl_Position = m * (vec4(-Size2, -Size2, 0.0,0.0) + gl_in[0].gl_Position);
	NewTexCoord = vec2(0.0,0.0);
	EmitVertex();


	//setup vertex 2
	gl_Position = m * (vec4(Size2, -Size2, 0.0,0.0) + gl_in[0].gl_Position);
	NewTexCoord = vec2(1.0,0.0);
	EmitVertex();


	//setup vertex 3
	gl_Position = m * (vec4(-Size2, Size2, 0.0,0.0) + gl_in[0].gl_Position);
	NewTexCoord = vec2(0.0,1.0);
	EmitVertex();

	//setup vertex 4
	gl_Position = m * (vec4(Size2, Size2, 0.0,0.0) + gl_in[0].gl_Position);
	NewTexCoord = vec2(1.0,1.0);
	EmitVertex();


	EndPrimitive();
}