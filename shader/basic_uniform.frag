#version 460

in vec4 camPosition;
in vec3 camNorm;
layout (location = 0) out vec4 FragColor;



//Light info
uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 Ld; //Diffuse
    vec3 Ls; //Specular
    } lights[3];


// Material info
uniform struct MaterialInfo{
    vec3 Ka; //Ambient
    vec3 Kd; //Diffuse
    vec3 Ks; //Specular
    float Shininess; //Shininess factor

} Material;


vec3 phong(int Light , vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 ambient = lights[Light].La*Material.Ka;

    vec3 s = normalize(vec3(lights[Light].Position-pos));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse = lights[Light].Ld*Material.Kd *sDotN;
    vec3 spec = vec3(0.0);
    if(sDotN>0.0){
        vec3 v = normalize(-pos.xyz);
        vec3 r = reflect(-s,n);
        spec = lights[Light].Ls*Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    
    return ambient+diffuse+spec;
}

vec3 blinnphong(int Light , vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 ambient = lights[Light].La*Material.Ka;

    vec3 s = normalize(vec3(lights[Light].Position-pos));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse = lights[Light].Ld*Material.Kd *sDotN;
    vec3 spec = vec3(0.0);
    if(sDotN>0.0){
        vec3 v = normalize(-pos.xyz);
        vec3 h = normalize(v+s);
        spec = lights[Light].Ls*Material.Ks*pow(max(dot(h,v),0.0),Material.Shininess);
    }
    
    return ambient+diffuse+spec;
}




void main() {
    vec3 Colour = vec3(0.0);
    for (int i = 0; i<3; i++)
    {
        Colour +=blinnphong(i,camNorm,camPosition);
    }
    FragColor = vec4(Colour, 1.0);
}
