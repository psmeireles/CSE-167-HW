#version 330 core
// This is a sample fragment shader.

in vec3 Normal;
in vec3 FragPos;

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

    vec3 color;

    float constant;
	float linear;
}; 

struct SpotLight {
    vec3 lightPos;
	vec3 lightDirection;
    
    float cutOff;

	vec3 color;

    float constant;
	float linear;
}; 
  
uniform Material material;
uniform PointLight pointlight;
uniform SpotLight spotlight;
uniform vec3 viewPos;
uniform int normalColor;

vec3 calcPointLight(PointLight pointlight){
	float distance = length(pointlight.lightPos - FragPos);
	float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance);

	// ambient
	vec3 ambient = pointlight.color * material.ambient;
  	
	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointlight.lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointlight.color * (diff * material.diffuse);
    
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pointlight.color * (spec * material.specular);  

	vec3 result = ambient + attenuation * (diffuse + specular);
	return result;
}

vec3 calcSpotLight(SpotLight spotlight){
	vec3 lightDir = normalize(spotlight.lightPos - FragPos);
	float theta = dot(lightDir, normalize(-spotlight.lightDirection));
	

	if(theta > spotlight.cutOff) 
	{
		float distance = length(spotlight.lightPos - FragPos);
		float attenuation = 1.0/(spotlight.constant + spotlight.linear*distance);
	
		// ambient
		vec3 ambient = spotlight.color * material.ambient;

		// diffuse 
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = spotlight.color * (diff * material.diffuse);

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = spotlight.color * (spec * material.specular);

		vec3 result = ambient + attenuation*(diffuse + specular);
		return result;
	}
	else  // else, use ambient light so scene isn't completely dark outside the spotlight.
		return vec3(spotlight.color * material.ambient);
}

void main()
{    
	if(normalColor == 1){
		FragColor = vec4((Normal.x + 1.0)/2.0, (Normal.y + 1.0)/2.0, (Normal.z + 1.0)/2.0, 1.0);
	}
	else{
		vec3 result = calcPointLight(pointlight);
		result += calcSpotLight(spotlight);
		FragColor = vec4(result, 1.0);
	}
}



