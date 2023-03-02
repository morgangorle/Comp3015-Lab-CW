#version 460

in vec4 camPosition;
in vec3 camNorm;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D baseTex;
layout (binding = 1) uniform sampler2D alphaTex;
layout (location = 0) out vec4 FragColor;



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
    vec3 texColour = texture(baseTex,TexCoord).rgb;
    
    //Handle Ambient Lighting
    vec3 ambient = Light.La*Material.Ka * texColour;
    vec3 diffuse= vec3(0);
    vec3 spec = vec3(0);

    vec3 s = normalize(vec3(Light.Position-pos));

    float sDotN = max(dot(s,n),0.0);
    diffuse = Light.Ld*Material.Kd *sDotN * texColour;
    spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(-pos.xyz);
     vec3 h = normalize(v+s);
     spec = Light.Ls*Material.Ks*pow(max(dot(h,v),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}




void main() {
    vec4 alphaMap = texture(alphaTex, TexCoord);
    if(alphaMap.a < 0.15){
        discard;
    }
    else{
        if(gl_FrontFacing){
            FragColor = vec4(blinnphong(camNorm,camPosition), 1.0);
        }
        else{
            FragColor = vec4(blinnphong(-camNorm,camPosition), 1.0);

        }
    }
}
