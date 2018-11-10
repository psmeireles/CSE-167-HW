#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexel;
layout (location = 2) in vec3 aNormal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;
out vec3 colorNormal;
out vec2 texel;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	colorNormal = mat3(transpose(inverse(model))) * vec3((aNormal.x + 1)/2, (aNormal.y + 1)/2, (aNormal.z + 1)/2);
	texel = aTexel;
	FragPos = vec3(model * vec4(aPos, 1.0));
}
