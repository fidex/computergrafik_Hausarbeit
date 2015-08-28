varying vec3 Normal;
varying vec3 Position;
varying vec3 lightDir;
varying vec2 Texcoord1;
varying vec2 Texcoord2;

void main()
{
    Position = (gl_ModelViewMatrix * gl_Vertex).xyz;
    Normal = gl_NormalMatrix*gl_Normal;
    Texcoord1 = gl_MultiTexCoord0.xy;
    Texcoord2 = gl_MultiTexCoord1.xy;
    lightDir = normalize(vec3(gl_LightSource[0].position));
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}