#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 TexCoords;

uniform mat4 TexGenMatCam0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vec4 VertexPos_eye =  model * view * vec4(aPos, 1.0);
	vec4 VertexPos_world = inverse(view) * VertexPos_eye;
		
	TexCoords = TexGenMatCam0 * VertexPos_world;

	gl_Position = projection * view * vec4(FragPos, 1.0);
}