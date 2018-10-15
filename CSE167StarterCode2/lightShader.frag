#version 330 core

in vec3 normal;

out vec4 color;

uniform vec3 lightPos;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    color = vec4(1.0);
}
