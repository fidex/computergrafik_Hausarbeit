#version 440

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 DiffColor;
uniform vec3 SpecColor;
uniform vec3 AmbientColor;
uniform float SpecExp;

uniform sampler2D DiffuseTexture;

in vec3 Normal;
in vec3 Position;
in vec2 Texcoord;

out vec4 fragColor;

bool funkymode = true;

float sat(float a)
{
	return clamp(a, 0.0, 1.0);
}

void main()
{
	vec3 N = normalize(Normal);
	vec3 L = normalize(LightPos - Position);
	vec3 E = normalize(-Position);
	vec3 R = reflect(-L, N);

	vec3 DiffuseComponent = DiffColor * sat(dot(N,L));
	vec3 SpecularComponent = SpecColor * pow(sat(dot(E,R)), SpecExp);

	vec3 DiffuseTexColor = texture2D(DiffuseTexture, Texcoord.xy).rgb;
	DiffuseComponent *= DiffuseTexColor;

	vec3 AmbientComponent = AmbientColor*DiffuseTexColor;
	
	fragColor = vec4(DiffuseComponent + SpecularComponent + AmbientComponent, 0.0);
	//fragColor = vec4(1.0, 0.0, 0.0, 0.0);

	/*if(funkymode){
		vec3 funkyPos = normalize(Position);
		fragColor = vec4(0.2, funkyPos.y*255, 0.2, 0.0);
	}*/

	/*if(dot(N,L) > 0.0){
        	fragColor = vec4(0.0, 0.0, 1.0, 0.0);
    	}else if(clamp(dot(N,L), 0.0, 1.0) != 0.0){
        	fragColor = vec4(1.0, 1.0, 0.0, 0.0);
    	}*/
}

