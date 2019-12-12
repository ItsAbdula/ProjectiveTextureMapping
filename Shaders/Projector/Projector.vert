#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec4 ProjTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 projectorBias;
uniform mat4 projectorProjection;
uniform mat4 projectorView;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{		
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal; 
	TexCoords = aTexCoords;
	
	mat4 InvViewMat = inverse(view);
	mat4 TexGenMatCam0 = projectorBias * projectorProjection * projectorView;
	
	vec4 posEye = view * model * vec4(aPos, 1.0);
	
	ProjTexCoords = TexGenMatCam0 * posEye;
	
	gl_Position = projection * view * vec4(FragPos, 1.0);
}