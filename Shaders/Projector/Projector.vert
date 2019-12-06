#version 330 core
uniform mat4 TexGenMatCam0;
uniform mat4 ViewMat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 InvViewMat = inverse(ViewMat);	
	
	vec4 posEye =  gl_ModelViewMatrix * gl_Vertex;
	vec4 posWorld = InvViewMat * posEye;
		
	gl_TexCoord[0] = TexGenMatCam0 * posWorld;

	gl_Position = ftransform();		
}