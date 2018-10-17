#include "OBJObject.h"
#include "Window.h"
#include "Light.h"

OBJObject::OBJObject(char* filepath)
{
	toWorld = glm::mat4(1.0f);
	min_x = std::numeric_limits<float>::max();
	min_y = std::numeric_limits<float>::max();
	min_z = std::numeric_limits<float>::max();
	max_x = std::numeric_limits<float>::lowest();
	max_y = std::numeric_limits<float>::lowest();
	max_z = std::numeric_limits<float>::lowest();

	objectColor = glm::vec3(1.0f, 0.41f, 0.7f);
	parse(filepath);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		sizeof(glm::vec3),                                // stride
		(GLvoid*)0                          // array buffer offset
	);

	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &normalBuffer);
}

void OBJObject::draw(GLuint shaderProgram)
{ 
	glUseProgram(shaderProgram);

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 model = toWorld;
	glm::mat4 view = Window::V;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uObjectColor = glGetUniformLocation(shaderProgram, "objectColor");
	uViewPos = glGetUniformLocation(shaderProgram, "viewPos");
	uLightPos = glGetUniformLocation(shaderProgram, "lightPos");
	uLightColor = glGetUniformLocation(shaderProgram, "lightColor");
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	// Now send these values to the shader program
	glUniform3fv(uObjectColor, 1, &objectColor[0]);
	glUniform3fv(uViewPos, 1, &Window::camPos[0]);
	glUniform3fv(uLightPos, 1, &Light::lightPos[0]);
	glUniform3fv(uLightColor, 1, &Light::lightColor[0]);
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &view[0][0]);
	// Now draw the OBJObject. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, indices[0], indices.size());
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::update()
{
	//spin(1.0f);
}

void OBJObject::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::scale(double x)
{
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(x));
}

void OBJObject::parse(const char *filepath)
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	char c1, c2;
	GLfloat x, y, z, r, g, b;

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
				this->colors.push_back(glm::vec3((x + 1.0f) / 2.0f, (y + 1.0f) / 2.0f, (z + 1.0f) / 2.0f));
			}
			else if (c2 == ' ') {
				fscanf(fp, " %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				this->vertices.push_back(glm::vec3(x, y, z));
				updateMinMaxCoordinates(x, y, z);
			}
		}
		else if (c1 == 'f') {
			GLuint f[6];
			fscanf(fp, " %d//%d %d//%d %d//$d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
			for(int i = 0; i < 6; i++)
				this->indices.push_back(f[i]-1);
		}
		c1 = fgetc(fp);
	}
	fclose(fp);

	shiftAndResizeModel();
}

void OBJObject::updateMinMaxCoordinates(float x, float y, float z)
{
	if (x > max_x) max_x = x;
	if (x < min_x) min_x = x;
	if (y > max_y) max_y = y;
	if (y < min_y) min_y = y;
	if (z > max_z) max_z = z;
	if (z < min_z) min_z = z;
}

void OBJObject::shiftAndResizeModel()
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