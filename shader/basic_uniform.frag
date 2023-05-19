#version 460


in vec3 FragNormal;
in vec4 FragPosition;
in vec2 FragTexCoord;
in vec3 FragVec;

layout (location = 0) out vec4 FragColour;

uniform sampler2D RenderTex;
layout(binding=2) uniform sampler2D chestTex;
layout(binding=3) uniform samplerCube skyBoxTex;
uniform int passNum;



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

//Fog info
uniform struct FogInfo {
    float MaxDist;
    float MinDist;
    vec3 Colour;

} Fog;

// A version of BlinnPhong Shading with Texturing
vec3 blinnphongTex(vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 texColour = texture(RenderTex, FragTexCoord).rgb;

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

vec3 blinnphongChest(vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 texColour = texture(chestTex, FragTexCoord).rgb;

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

//A version of BlinnPhong shading with no texturing.
vec3 blinnphong(vec3 n, vec4 pos){
    //Handle Ambient Lighting

    vec3 ambient = Light.La;
    vec3 s = normalize(vec3(Light.Position.xyz-pos.xyz));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse= Light.Ld * sDotN * Material.Kd;
    vec3 spec = vec3(0.0);
   if(sDotN>0.0){
     vec3 v = normalize(-pos.xyz);
     vec3 h = normalize(v+s);
     spec = Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
   }
    return ambient+diffuse+spec;
}

vec4 spot()
{

    //return vec4(blinnphongTex(FragNormal,FragPosition), 1.0);
    float dist = abs(FragPosition.z);
    float fogFactor = (Fog.MaxDist-dist) / (Fog.MaxDist-Fog.MinDist);
    fogFactor = clamp(fogFactor,0.0,1.0);
    vec3 shadeColour = blinnphongTex(FragNormal,FragPosition);
    vec3 colour = mix(Fog.Colour, shadeColour,fogFactor);
    return vec4(colour,1.0);
}

vec4 noSpot()
{
    //return vec4(blinnphong(FragNormal,FragPosition), 1.0);
    float dist = abs(FragPosition.z);
    float fogFactor = (Fog.MaxDist-dist) / (Fog.MaxDist-Fog.MinDist);
    fogFactor = clamp(fogFactor,0.0,1.0);
    vec3 shadeColour = blinnphong(FragNormal,FragPosition);
    vec3 colour = mix(Fog.Colour, shadeColour,fogFactor);
    return vec4(colour,1.0);
}
// A function called for my skybox.
vec4 skyBox()
{
    vec3 texColour = texture(skyBoxTex, normalize(FragVec)).rgb;
    texColour = pow(texColour, vec3(1.0/2.2));
    return vec4(texColour,1);

}
// A function called to render the fog.
vec4 FogRender()
{
    float dist = abs(FragPosition.z);
    float fogFactor = (Fog.MaxDist-dist) / (Fog.MaxDist-Fog.MinDist);
    fogFactor = clamp(fogFactor,0.0,1.0);
    vec3 shadeColour = blinnphong(FragNormal,FragPosition);
    vec3 colour = mix(Fog.Colour, shadeColour,fogFactor);
    return vec4(colour,1.0);

}

vec4 ChestRender()
{
    float dist = abs(FragPosition.z);
    float fogFactor = (Fog.MaxDist-dist) / (Fog.MaxDist-Fog.MinDist);
    fogFactor = clamp(fogFactor,0.0,1.0);
    vec3 shadeColour = blinnphongChest(FragNormal,FragPosition);
    vec3 colour = mix(Fog.Colour, shadeColour,fogFactor);
    return vec4(colour,1.0);

}


void main() {
    //The passNum uniform is used to determine which shading function to use.
    if(passNum == 0)
    {
        FragColour = noSpot();
    }
    else if (passNum == 1)
    {
        FragColour = spot();
    }
    else if (passNum == 2)
    {
        FragColour = skyBox();
    }

    else if (passNum == 3)
    {
        FragColour = ChestRender();
    }
}
