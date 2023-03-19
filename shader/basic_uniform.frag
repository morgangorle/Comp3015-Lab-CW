#version 460


in vec3 Normal;
in vec4 Position;

layout (binding = 0)uniform sampler2D RenderTex;

uniform float EdgeThreshold;
uniform int Pass;

layout( location = 0 ) out vec4 FragColor;

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


vec3 blinnphong(vec3 n, vec4 pos){
    //Handle Ambient Lighting

    vec3 ambient = Light.La;
    vec3 s = normalize(vec3(Light.Position.xyz-pos.xyz));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse= Material.Kd * sDotN * Light.L;
    vec3 spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(-pos.xyz);
     vec3 h = normalize(v+s);
     spec = Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}

float luminance( vec3 color )
{
 return dot(lum,color);
}

vec4 pass1()
{
 return vec4(blinnphong(normalize(Normal), Position ),1.0);
}
vec4 pass2()
{
 ivec2 pix = ivec2(gl_FragCoord.xy); //we grab a pixel to check if edge
//pick neighboutring pixels for convolution filter
//check lecture slides
 float s00 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,1)).rgb);
 float s10 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,0)).rgb);
 float s20 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,-1)).rgb);
 float s01 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,1)).rgb);
 float s21 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,-1)).rgb);
 float s02 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,1)).rgb);
 float s12 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,0)).rgb);
 float s22 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,-1)).rgb);
 float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
 float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
 float g = sx * sx + sy * sy;
 if( g > EdgeThreshold )
 return vec4(1.0); //edge
 else
 return vec4(0.0,0.0,0.0,1.0); //no edge
}


void main() {
    if( Pass == 1 ) FragColor = pass1();
    if( Pass == 2 ) FragColor = pass2();
}
