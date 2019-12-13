#version 330 core

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec4 ProjTexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D projImage;

void main (void)
{
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

	vec4 result = vec4(ambient + diffuse + specular, 1.0);
	
	vec4 projColor;
    if( ProjTexCoords.q > 0.0 )
    {    
		vec2 finalCoords = ProjTexCoords.st / ProjTexCoords.q;
		vec4 ProjMapColor_forCam1 = texture(projImage, finalCoords);
		projColor = ProjMapColor_forCam1;
		
		if(ProjMapColor_forCam1.a < 1.0)
		{
			projColor = result;
		}	
    }
	
    FragColor = mix(result, projColor, 0.4f);
}