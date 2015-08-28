uniform int farbe;

varying vec3 normal, lightDir;

void main()
{
    float intensity;
    vec4 color;
    intensity = dot(lightDir,normal);
    if(farbe == 1){
        if (intensity > 0.95)
            color = vec4(1.0,0.0,0.0,1.0);
        else if (intensity > 0.5)
            color = vec4(0.75,0.0,0.0,1.0);
        else if (intensity > 0.25)
            color = vec4(0.5,0.0,0.0,1.0);
        else
            color = vec4(0.25,0.0,0.0,1.0);
    }
    else{
        if (intensity > 0.95)
            color = vec4(0.0,1.0,0.0,1.0);
        else if (intensity > 0.5)
            color = vec4(0.0,1.75,0.0,1.0);
        else if (intensity > 0.25)
            color = vec4(0.0,0.5,0.0,1.0);
        else
            color = vec4(0.0,0.25,0.0,1.0);
    }
    gl_FragColor = color;
    
}