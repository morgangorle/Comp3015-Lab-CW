#version 460


in vec3 Normal;
in vec4 Position;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColour;

uniform sampler2D RenderTex;



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
    vec3 texColour = texture(RenderTex, TexCoord).rgb;

    vec3 ambient = Light.La*texColour;
    vec3 s = normalize(vec3(Light.Position.xyz-pos.xyz));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse= texColour * sDotN;
    vec3 spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(-pos.xyz);
     vec3 h = normalize(v+s);
     spec = Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}


void main() {

    FragColour = vec4(blinnphong(Normal,Position), 1.0);
}
