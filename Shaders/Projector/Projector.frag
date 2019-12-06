#version 330 core
uniform sampler2D projMap_forCam1;

void main (void)
{
    vec4 final_color = vec4(0.0, 0.0, 0.0, 1.0);
    if( gl_TexCoord[0].q &gt; 0.0 )
    {
		vec4 ProjMapColor_forCam1 = texture2DProj(projMap_forCam1, gl_TexCoord[0]);
		final_color = ProjMapColor_forCam1;			
    }

		
    gl_FragColor = final_color;			
}