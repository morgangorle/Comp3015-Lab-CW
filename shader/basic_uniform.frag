#version 460


in vec3 Normal;
in vec4 Position;
in vec2 TexCoord;


uniform int Pass;

layout(binding=0) uniform sampler2D PositionTex;
layout(binding=1) uniform sampler2D NormalTex;
layout(binding=2) uniform sampler2D ColorTex;


// XYZ/RGB conversion matrices from: -- Useful resource
// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
uniform mat3 rgb2xyz = mat3(
0.4124564, 0.2126729, 0.0193339,
0.3575761, 0.7151522, 0.1191920,
0.1804375, 0.0721750, 0.9503041 );

uniform mat3 xyz2rgb = mat3(
3.2404542, -0.9692660, 0.0556434,
-1.5371385, 1.8760108, -0.2040259,
-0.4985314, 0.0415560, 1.0572252 );

uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform float AveLum;

uniform float LumThresh;
uniform float PixOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[10];

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 PositionData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 ColorData;

const vec3 lum = vec3(0.2126, 0.7152, 0.0722);


//Light info
uniform struct lightinfo
{
    vec4 Position;
    vec3 La;
    vec3 L; //Ld = Ls
} Light;

// Material info
uniform struct MaterialInfo{
    vec3 Ka; //Ambient
    vec3 Kd; //Diffuse
    vec3 Ks; //Specular
    float Shininess; //Shininess factor

} Material;

float luminance( vec3 color )
{
return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

vec3 blinnphong(vec3 n, vec4 pos, vec3 diffColour){
    //Handle Ambient Lighting

    vec3 ambient = Light.La;
    vec3 s = normalize(vec3(Light.Position.xyz-pos.xyz));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse= diffColour * sDotN;
    vec3 spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(-pos.xyz);
     vec3 h = normalize(v+s);
     spec = Light.L * Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}


void pass1()
{
// Store position, normal, and diffuse color in textures
PositionData = Position;
NormalData = normalize(Normal);
ColorData = Material.Kd;
}
void pass2()
{
// Retrieve position and normal information from textures
vec4 pos = vec4( texture( PositionTex, TexCoord ) );
vec3 norm = vec3( texture( NormalTex, TexCoord ) );
vec3 diffColor = vec3( texture(ColorTex, TexCoord) );
//FragColor = vec4( diffuseModel(pos,norm,diffColor), 1.0 );
FragColor = vec4( blinnphong(norm,pos,diffColor), 1.0 );
}



void main()
{
    if( Pass == 1 )
    {
    pass1();
    }
    else if( Pass == 2 )
    {
    pass2();
    }
}
