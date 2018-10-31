#version 330 core
// This is a sample fragment shader.

in vec3 Normal;
in vec3 FragPos;
in vec3 colorNormal;
in vec2 texel;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform int normalColor;

void main()
{    
	if(normalColor == 1)
		FragColor = vec4(Normal, 1.0);
	else
		FragColor = texture(ourTexture, texel);
}



