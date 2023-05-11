#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 GNormal;
out vec3 GPosition;

noperspective out vec3 GEdgeDistance;

in vec3 VNormal[];
in vec3 VPosition[];

uniform mat4 ViewportMatrix;

void main()
{
	mat4 m = ProjectionMatrix;

	//setup vertex 1
	gl_Position = m * (vec4(-Size2, -Size2, 0.0,0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(0.0,0.0);
	EmitVertex();


	//setup vertex 2
	gl_Position = m * (vec4(Size2, -Size2, 0.0,0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(1.0,0.0);
	EmitVertex();


	//setup vertex 3
	gl_Position = m * (vec4(-Size2, Size2, 0.0,0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(0.0,1.0);
	EmitVertex();

	//setup vertex 4
	gl_Position = m * (vec4(Size2, Size2, 0.0,0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(1.0,1.0);
	EmitVertex();


	EndPrimitive();
}