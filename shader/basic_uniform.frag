#version 460


in vec3 EyeNormal;
in vec4 EyePosition;
in vec4 ProjTexCoord;

layout (binding = 0) uniform sampler2D ProjectorTex;

layout (location = 0) out vec4 FragColour;



//Light info
uniform struct lightinfo
{
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
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
    vec3 ambient = Light.La*Material.Ka;
    vec3 diffuse= vec3(0);
    vec3 spec = vec3(0);

    vec3 s = normalize(vec3(Light.Position-pos));

    float sDotN = max(dot(s,n),0.0);
    diffuse = Light.Ld*Material.Kd *sDotN;
    spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(-pos.xyz);
     vec3 h = normalize(v+s);
     spec = Light.Ls*Material.Ks*pow(max(dot(h,v),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}
/*
// A version with no Pos
vec3 blinnphong(vec3 n){
    vec3 texColour = texture(ColorTex,TexCoord).rgb;
    
    //Handle Ambient Lighting
    vec3 ambient = Light.La*Material.Ka * texColour;
    vec3 diffuse= vec3(0);
    vec3 spec = vec3(0);

    vec3 s = normalize(LightDir);

    float sDotN = max(dot(s,n),0.0);
    diffuse = Light.Ld*Material.Kd *sDotN * texColour;
    spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(ViewDir);
     vec3 h = normalize(v+s);
     spec = Light.Ls*Material.Ks*pow(max(dot(h,v),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}
*/


void main() {
    vec3 colour = blinnphong(EyeNormal,normalize(EyePosition));
    vec3 projTexColor = vec3(0.0);
    if( ProjTexCoord.z > 0.0 ){
        projTexColor = textureProj( ProjectorTex, ProjTexCoord ).rgb;
    
    }

    FragColour = vec4(colour + projTexColor * 0.5, 1);
}
