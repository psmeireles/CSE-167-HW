#include "PointLight.h"
#include "Window.h"

glm::mat4 PointLight::toWorld;
glm::vec3 PointLight::lightPos, PointLight::currColor, PointLight::actualColor;
float PointLight::constant, PointLight::linear;

PointLight::PointLight()
{
	toWorld = glm::mat4(1.0f);
	actualColor = glm::vec3(1.0f, 1.0f, 0.0f);
	currColor = actualColor;
	lightPos = glm::vec3(0.0f, 1.0f, 2.0f);
	constant = 1.0f;
	linear = 0.22f;

	parse("../obj/sphere.obj");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	   
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);


	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void PointLight::parse(char* filepath) {
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	char c1, c2;
	GLfloat x, y, z;

	FILE *fp = fopen(filepath, "rb");
	if (fp == NULL) {
		printf("error loading file\n");
		system("pause");
		exit(-1);
	}

	c1 = fgetc(fp);
	while (c1 != EOF) {
		if (c1 == 'v') {
			c2 = fgetc(fp);

			if (c2 == 'n') {
				fscanf(fp, " %f %f %f", &x, &y, &z);
				this->normals.push_back(glm::vec3(x, y, z));
			}
			else if (c2 == ' ') {
				fscanf(fp, " %f %f %f", &x, &y, &z);
				this->vertices.push_back(glm::vec3(x, y, z));
				updateMinMaxCoordinates(x, y, z);
			}
		}
		else if (c1 == 'f') {
			GLuint f[6];
			fscanf(fp, " %u//%u %u//%u %u//$u", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
			for (int i = 0; i < 3; i++)
				this->indices.push_back(f[2*i] - 1);
		}
		c1 = fgetc(fp);
	}
	fclose(fp);

	shiftAndResizeModel();
}

void PointLight::updateMinMaxCoordinates(float x, float y, float z)
{
	if (x > max_x) max_x = x;
	if (x < min_x) min_x = x;
	if (y > max_y) max_y = y;
	if (y < min_y) min_y = y;
	if (z > max_z) max_z = z;
	if (z < min_z) min_z = z;
}

void PointLight::shiftAndResizeModel()
{
	// Find center of model
	GLfloat avg_x = (max_x + min_x) / 2.0f;
	GLfloat avg_y = (max_y + min_y) / 2.0f;
	GLfloat avg_z = (max_z + min_z) / 2.0f;

	// Shifting max and mins
	max_x -= avg_x;
	min_x -= avg_x;
	max_y -= avg_y;
	min_y -= avg_y;
	max_z -= avg_z;
	min_z -= avg_z;

	// Finding max coordinate
	GLfloat max_abs_x = abs(max_x) > abs(min_x) ? abs(max_x) : abs(min_x);
	GLfloat max_abs_y = abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y);
	GLfloat max_abs_z = abs(max_z) > abs(min_z) ? abs(max_z) : abs(min_z);

	GLfloat max_coord = max_abs_x;
	if (max_coord < max_abs_y) {
		max_coord = max_abs_y;
	}
	if (max_coord < max_abs_z) {
		max_coord = max_abs_z;
	}

	// Shifting and resizing all vertices
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x -= avg_x;
		vertices[i].x /= max_coord;
		vertices[i].y -= avg_y;
		vertices[i].y /= max_coord;
		vertices[i].z -= avg_z;
		vertices[i].z /= max_coord;
	}

	// Resizing max and mins
	max_x /= max_coord;
	min_x /= max_coord;
	max_y /= max_coord;
	min_y /= max_coord;
	max_z /= max_coord;
	min_z /= max_coord;
}

PointLight::~PointLight()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void PointLight::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 model = glm::scale(glm::translate(toWorld, lightPos), glm::vec3(0.2f));
	glm::mat4 view = Window::V;
	glm::mat4 modelview = view * model;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uColor = glGetUniformLocation(shaderProgram, "color");
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	//uLightColor = glGetUniformLocation(shaderProgram, "lightColor");
	// Now send these values to the shader program
	glUniform3fv(uColor, 1, &PointLight::currColor[0]);
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	//glUniform3fv(uLightColor, 1, &lightColor[0]);
	// Now draw the OBJObject. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void PointLight::update()
{
	//spin(1.0f);
}
