#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>


class Material
{
public:
	Material();
	Material(glm::vec3 ambient,	glm::vec3 diffuse, glm::vec3 specular, float shininess);
	~Material();

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

#endif