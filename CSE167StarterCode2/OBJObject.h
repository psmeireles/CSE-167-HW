#ifndef _OBJOBJECT_H_
#define _OBJOBJECT_H_

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

class OBJObject
{
private:
	std::vector<GLuint> indices2;
	std::vector<glm::vec3> vertices2;
	std::vector<glm::vec3> normals2;
	std::vector<glm::vec3> colors;
	GLfloat min_x, max_x;
	GLfloat min_y, max_y;
	GLfloat min_z, max_z;

	void updateMinMaxCoordinates(float x, float y, float z);
	void shiftAndResizeModel();
public:
	OBJObject(char* filepath);
	~OBJObject();

	glm::mat4 toWorld;

	glm::vec3 lightColor;

	void draw(GLuint);
	void update();
	void spin(float);
	void scale(double);
	void parse(const char* filepath);


	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview, uLightColor;
};

#endif

