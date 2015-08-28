uniform sampler2D Sand;
uniform sampler2D Gras;
uniform sampler2D Mixmap;

varying vec3 Normal;
varying vec3 lightDir;
varying vec3 Position;
varying vec2 Texcoord1;
varying vec2 Texcoord2;

float sat (float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
    vec3 SandColor = texture2D(Sand  , Texcoord2).rgb;
    vec3 GrasColor = texture2D(Gras  , Texcoord2).rgb;
    vec3 Mixmap    = texture2D(Mixmap, Texcoord1).rgb;
    vec3 Mixmap2   = vec3(1.0 - Mixmap.x,1.0 - Mixmap.y,1.0 - Mixmap.z);
    vec3 color = SandColor * Mixmap + GrasColor * Mixmap2;
    
    
    vec3 LightColor = color;//vec3(1.0, 0.0, 0.0);
    vec3 DiffColor = color;//vec3(0.0, 0.0, 0.5);
    vec3 SpecColor = color*0.5;//vec3(0.0, 0.0, 1.0);
    vec3 AmbientColor = color*0.0;//vec3(0.0, 0.0, 0.25);
    float SpecExp = 10.0;
    
    
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightDir);
    vec3 E = normalize(-Position);
    vec3 R = reflect(-L, N);
    
    vec3 DiffuseComponent = DiffColor*sat(dot(N,L));
    vec3 SpecularComponent =SpecColor *pow(sat(dot(E,R)),SpecExp);
    
    vec3 DiffuseTexColor = color;
    DiffuseComponent*= DiffuseTexColor;
    
    vec3 AmbientComponent = AmbientColor*DiffuseTexColor;
    gl_FragColor = vec4(DiffuseComponent + SpecularComponent + AmbientComponent,0);
    
    
    
////    compute diffuse lighting (directional lightsource only)
//    float diffuse = max(dot(gl_LightSource[0].position.xyz, Normal), 0.0);
//    gl_FragColor = vec4(diffuse * color + gl_LightSource[0].ambient.xyz, 1.0);
//    gl_FragColor = vec4(color.rgb, 1.0);
}