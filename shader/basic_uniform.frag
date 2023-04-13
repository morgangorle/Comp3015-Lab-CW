#version 460

in vec2 TexCoord;

uniform sampler2D SpriteTex;

layout(location = 0) out vec4 FragColor;




//vec3 blinnphong(vec3 n, vec4 pos, vec3 diffColour){
    //Handle Ambient Lighting

    //vec3 ambient = Light.La;
    //vec3 s = normalize(vec3(Light.Position.xyz-pos.xyz));
    //float sDotN = max(dot(s,n),0.0);
    //vec3 diffuse= diffColour * sDotN;
    //vec3 spec = vec3(0.0);
   //if(sDotN>0.0){
     //vec3 v = normalize(-pos.xyz);
     //vec3 h = normalize(v+s);
     //spec = Light.L * Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
   //}
    //return ambient+diffuse+spec;
//}
void main()
{
    FragColor = texture(SpriteTex, TexCoord);
}
