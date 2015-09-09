#version 440 compatibility

in layout(location=0) vec3 position;
in layout(location=1) vec3 normal;
in layout(location=2) vec2 texcoord;

out vec3 Normal;
out vec3 Position;
out vec2 Texcoord;

void main()
{
	Position = (gl_ModelViewMatrix * vec4(position, 1.0)).xyz;
	//Position = position;
	Normal = gl_NormalMatrix * normal;
	Texcoord = texcoord;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
}

