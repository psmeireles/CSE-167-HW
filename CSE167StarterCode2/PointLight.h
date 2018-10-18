
#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class PointLight
{

public:
	PointLight();
	~PointLight();

	static glm::mat4 toWorld;

	static glm::vec3 lightPos;
	static glm::vec3 actualColor;
	static glm::vec3 currColor;
	static float constant, linear;

	void draw(GLuint);
	void update();
	void parse(char*);
	void updateMinMaxCoordinates(float x, float y, float z);
	void shiftAndResizeModel();

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview, uColor;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	GLfloat min_x, max_x;
	GLfloat min_y, max_y;
	GLfloat min_z, max_z;
};

#endif

