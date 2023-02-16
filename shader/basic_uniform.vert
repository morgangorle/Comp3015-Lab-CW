#version 460
//Input variables
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;


//Output variables
out vec3 LightIntensity;
out vec3 Colour;

// Uniform Variables

//Light info

uniform struct LightInfo {
vec4 Position; // Light position in eye coords.
vec3 La; // Ambient light intensity
vec3 L; // Diffuse and specular light intensity
} lights[3];

//Old Light Variables
//uniform vec4 LightPosition;
//uniform vec3 Ld; //Diffuse
//uniform vec3 La; //Ambient
//uniform vec3 Ls; //Specular


// Material info
uniform struct MaterialInfo {
vec3 Ka; // Ambient reflectivity
vec3 Kd; // Diffuse reflectivity
vec3 Ks; // Specular reflectivity
float Shininess; // Specular shininess factor
} Material;
//uniform vec3 Kd; //Diffuse
//uniform vec3 Ka; //Ambient
//uniform vec3 Ks; //Specular
//uniform float Shininess;


//Matrixes
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

// Using the phong model
vec3 phongModel( int light, vec4 position, vec3 n )
{
//calculate ambient here, to access each light La value use lights[light].La:
vec3 ambient = lights[light].La * Material.Ka;
//calculate diffuse here
vec3 s = lights[light].L * Material.Ks; //find out s vector
float sDotN = dot(s,n);  //calculate dot product between s & n
vec3 diffuse = Material.Kd * sDotN;
//calculate specular here
vec3 spec = vec3(0.0);
if( sDotN > 0.0 )
{
vec3 v = normalize(-position.xyz);
vec3 r = reflect( -s, n );
spec = Material.Ks * pow( max( dot(r,v), 0.0 ),
Material.Shininess );
}
return ambient + lights[light].L * (diffuse + spec);
};


void main()
{
    vec3 n = normalize( NormalMatrix * VertexNormal);
    vec4 camCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    Colour = vec3(0.0);
    for( int i = 0; i < 3; i++ )
    Colour += phongModel( i, camCoords, n );

    gl_Position = MVP * vec4(VertexPosition,1.0);

    //vec3 n = normalize(NormalMatrix*VertexNormal);
    //vec4 pos = ModelViewMatrix * vec4(VertexPosition,1.0);
    //vec3 s = normalize(vec3(LightPosition-pos));
    //LightIntensity = Kd * Ld * max(dot(s,n),0.0);
    //gl_Position = MVP*vec4(VertexPosition,1.0);
};


