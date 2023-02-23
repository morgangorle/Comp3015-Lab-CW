#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

//Light info
uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 Ld; //Diffuse
    vec3 Ls; //Specular
    } Light;


// Material info
uniform struct MaterialInfo{
    vec3 Ka; //Ambient
    vec3 Kd; //Diffuse
    vec3 Ks; //Specular
    float Shininess; //Shininess factor

} Material;


//Matrixes
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

vec3 phong(vec3 n, vec4 pos){
    //Handle Ambient Lighting
    vec3 ambient = Light.La*Material.Ka;

    vec3 s = normalize(vec3(Light.Position-pos));
    float sDotN = max(dot(s,n),0.0);
    vec3 diffuse = Light.Ld*Material.Kd *sDotN;
    vec3 spec = vec3(0.0);
    if(sDotN>0.0){
        vec3 v = normalize(-pos.xyz);
        vec3 r = reflect(-s,n);
        spec = Light.Ls*Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    
    return ambient+diffuse+spec;
}

void getCamSpaceValues(out vec3 norm, out vec4 position){
    norm = normalize(NormalMatrix*VertexNormal);
    position = ModelViewMatrix * vec4(VertexPosition,1.0);
}

void main()
{
    vec3 camNorm;
    vec4 camPosition;
    getCamSpaceValues(camNorm,camPosition);

    LightIntensity = phong(camNorm,camPosition);
    gl_Position = MVP*vec4(VertexPosition,1.0);
}
