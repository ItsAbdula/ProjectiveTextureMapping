#version 330 core
uniform sampler2D image;

out vec4 FragColor;

in vec4 TexCoords;

void main (void)
{
	vec4 final_color;
    if( TexCoords.q > 0.0 )
    {    
		vec2 finalCoords = TexCoords.st / TexCoords.q;
		vec4 ProjMapColor_forCam1 = texture(image, finalCoords);
		final_color = ProjMapColor_forCam1;			
    }

    FragColor = final_color;
}