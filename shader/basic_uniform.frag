#version 460

in vec4 camPosition;
in vec3 camNorm;
layout (location = 0) out vec4 FragColor;



//Light info
uniform struct SpotlightInfo
{
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
    vec3 Direction;
    float Exponent;
    float Cutoff;

} Spot;

const int level = 5;

const float scaleFactor = 1.0/level;


// Material info
uniform struct MaterialInfo{
    vec3 Ka; //Ambient
    vec3 Kd; //Diffuse
    vec3 Ks; //Specular
    float Shininess; //Shininess factor

} Material;


vec3 phong(vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 ambient = Spot.La*Material.Ka;
    vec3 diffuse= vec3(0);
    vec3 spec = vec3(0);

    vec3 s = normalize(vec3(Spot.Position-pos));
    float cosAng = dot(-s,normalize(Spot.Direction));
    float angle = acos(cosAng);
    float spotScale = 0.0;

    if (angle < Spot.Cutoff){
        spotScale = pow(cosAng , Spot.Exponent);
        float sDotN = max(dot(s,n),0.0);
        diffuse = Spot.Ld*Material.Kd *sDotN;
        spec = vec3(0.0);
        if(sDotN>0.0){
            vec3 v = normalize(-pos.xyz);
            vec3 r = reflect(-s,n);
            spec = Spot.Ls*Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
        }
    }

    
    return ambient+diffuse+spec;
}

vec3 blinnphong(vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 ambient = Spot.La*Material.Ka;
    vec3 diffuse= vec3(0);
    vec3 spec = vec3(0);

    vec3 s = normalize(vec3(Spot.Position-pos));

    float cosAng = dot(-s,normalize(Spot.Direction));
    float angle = acos(cosAng);
    float spotScale = 0.0;

    if (angle < Spot.Cutoff){
        spotScale = pow(cosAng , Spot.Exponent);
        float sDotN = max(dot(s,n),0.0);
        diffuse = Spot.Ld*Material.Kd *(floor(sDotN* level)* scaleFactor);
        spec = vec3(0.0);
        if(sDotN>0.0){
            vec3 v = normalize(-pos.xyz);
            vec3 h = normalize(v+s);
            spec = Spot.Ls*Material.Ks*pow(max(dot(h,v),0.0),Material.Shininess);
        }


    }   
    return ambient+diffuse+spec;
}




void main() {
    FragColor = vec4(blinnphong(camNorm,camPosition), 1.0);
}
