#version 460

in vec4 camPosition;
in vec3 camNorm;
layout (location = 0) out vec4 FragColor;



//Light info
uniform struct lightinfo
{
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
} Light;


uniform struct FogInfo {
    float MaxDist;
    float MinDist;
    vec3 Colour;

} Fog;


// Material info
uniform struct MaterialInfo{
    vec3 Ka; //Ambient
    vec3 Kd; //Diffuse
    vec3 Ks; //Specular
    float Shininess; //Shininess factor

} Material;


vec3 phong(vec3 n, vec4 pos){
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
        vec3 r = reflect(-s,n);
        spec = Light.Ls*Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
       }
    

    
    return ambient+diffuse+spec;
}

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




//void main() {
    //FragColor = vec4(blinnphong(camNorm,camPosition), 1.0);
//}

void main(){
    float dist = abs( camPosition.z);

    float fogFactor = (Fog.MaxDist-dist) / (Fog.MaxDist-Fog.MinDist);
    fogFactor = clamp(fogFactor,0.0,1.0);
    vec3 shadeColour = blinnphong(normalize(camNorm),camPosition);
    vec3 colour = mix(Fog.Colour, shadeColour,fogFactor);
    FragColor = vec4(colour,1.0);

}
