#version 460


in vec3 Normal;
in vec4 Position;

layout (binding = 0)uniform sampler2D Texture0;

uniform float EdgeThreshold;
uniform float Weight[5];
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
ivec2 pix = ivec2( gl_FragCoord.xy );
vec4 sum = texelFetch(Texture0, pix, 0) * Weight[0];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,1) ) * Weight[1];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,-1) ) * Weight[1];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,2) ) * Weight[2];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,-2) ) * Weight[2];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,3) ) * Weight[3];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,-3) ) * Weight[3];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,4) ) * Weight[4];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(0,-4) ) * Weight[4];
return sum;
}

vec4 pass3()
{
ivec2 pix = ivec2( gl_FragCoord.xy );
vec4 sum = texelFetch(Texture0, pix, 0) * Weight[0];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(1,0) ) * Weight[1];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(-1,0) ) * Weight[1];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(2,0) ) * Weight[2];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(-2,0) ) * Weight[2];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(3,0) ) * Weight[3];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(-3,0) ) * Weight[3];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(4,0) ) * Weight[4];
sum += texelFetchOffset( Texture0, pix, 0, ivec2(-4,0) ) * Weight[4];
return sum;
}



void main()
{
    if( Pass == 1 )
    {
    FragColor = pass1();
    }
    else if( Pass == 2 )
    {
    FragColor = pass2();
    }
    else if( Pass == 3 )
    {
    FragColor = pass3();
    }
}
