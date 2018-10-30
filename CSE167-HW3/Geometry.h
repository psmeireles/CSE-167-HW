#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_


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
#include "Material.h"
#include "Window.h"

class Geometry:public Node
{
private:
	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> texIndices;
	std::vector<GLuint> normalIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texels;
	GLfloat min_x, max_x;
	GLfloat min_y, max_y;
	GLfloat min_z, max_z;

	void updateMinMaxCoordinates(float x, float y, float z);
	void shiftAndResizeModel();
	void loadTexture();
public:
	Geometry(char* filepath);
	~Geometry();

	void update();
	void spin(float);
	void scale(double);
	void parse(const char* filepath);
	void draw(GLuint shaderProgram, glm::mat4 C);

	glm::mat4 toWorld;

	GLuint VBO, VAO, EBO, normalBuffer, texBuffer;
};

#endif