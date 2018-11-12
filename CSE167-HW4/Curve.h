#ifndef _CURVES_H_
#define _CURVES_H_


#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include <vector>
#include "Node.h"
#include "Window.h"

class Curve :public Node
{
private:
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texels;

	void shiftAndResizeModel();
	void loadTexture();
public:
	Curve(std::vector<glm::vec3> controlPoints);
	~Curve();

	void update();
	void spin(float);
	void scale(double);
	void parse(const char* filepath);
	void draw(GLuint shaderProgram, glm::mat4 C);
	void updateMinMaxCoordinates(float x, float y, float z);
	void shiftAndResizeSphere();

	glm::mat4 toWorld;

	GLuint VBO, VAO, EBO, normalBuffer, texBuffer;
};

#endif