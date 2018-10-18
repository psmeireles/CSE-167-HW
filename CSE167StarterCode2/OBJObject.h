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
#include "Material.h"

class OBJObject
{
private:
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	GLfloat min_x, max_x;
	GLfloat min_y, max_y;
	GLfloat min_z, max_z;
	Material material;

	void updateMinMaxCoordinates(float x, float y, float z);
	void shiftAndResizeModel();
public:
	OBJObject(char* filepath, Material material);
	~OBJObject();

	glm::mat4 toWorld;

	static int normalColor;

	void draw(GLuint);
	void update();
	void spin(float);
	void scale(double);
	void parse(const char* filepath);


	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, normalBuffer;
	GLuint uProjection, uModel, uView, uViewPos, uMAmbient;
	GLuint uPointLightPos, uPointLightColor, uPointLightConstant, uPointLightLinear;
	GLuint uSpotLightPos, uSpotLightDirection, uSpotLightCutOff, uSpotLightOuterCutOff, uSpotLightColor, uSpotLightConstant, uSpotLightLinear;
	GLuint uMDiffuse, uMSpecular, uMShininess;
	GLuint uNormalColor;
};

#endif

