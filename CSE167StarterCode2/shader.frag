#version 330 core
// This is a sample fragment shader.

in vec3 Normal;
in vec3 FragPos;
in vec3 Color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct PointLight {
    vec3 lightPos;

    vec3 ambient;
    vec3 diffuse;
	vec3 specular;

    float constant;
	float linear;
	float quadratic;
}; 

struct SpotLight {
    vec3 lightPos;
    vec3 lightColor;
	vec3 lightDirection;
    
    float cutOff;
}; 
  
uniform Material material;
uniform PointLight pointlight;
uniform SpotLight spotlight;
uniform vec3 viewPos;
uniform int normalColor;


void main()
{    
	if(normalColor == 1){
		FragColor = vec4(Color, 1.0);
	}
	else{
		float distance = length(pointlight.lightPos - FragPos);
		float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + pointlight.quadratic * distance * distance);

		// ambient
		vec3 ambient = pointlight.ambient * material.ambient;
  	
		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(pointlight.lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = pointlight.diffuse * (diff * material.diffuse);
    
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = pointlight.specular * (spec * material.specular);  

		vec3 result = attenuation * (ambient + diffuse + specular);
		FragColor = vec4(result, 1.0);
	}
}

