#include "SpotLight.h"
#include "Window.h"

glm::vec3 SpotLight::lightPos, SpotLight::lightColor, SpotLight::lightDirection;
float SpotLight::cutOff;

SpotLight::SpotLight()
{
	toWorld = glm::mat4(1.0f);
	lightColor = glm::vec3(0.0f, 0.0f, 1.0f);
	lightPos = Window::camPos;
	lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	cutOff = glm::cos(glm::radians(12.5f));

	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(VAO);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
}

SpotLight::~SpotLight()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
}

void SpotLight::draw(GLuint shaderProgram)
{
	//glUseProgram(shaderProgram);
	//// Calculate the combination of the model and view (camera inverse) matrices
	//glm::mat4 model = glm::scale(glm::translate(toWorld, lightPos), glm::vec3(0.2f));
	//glm::mat4 view = Window::V;
	//glm::mat4 modelview = view * model;

	//// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	//// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	//// Get the location of the uniform variables "projection" and "modelview"
	//uProjection = glGetUniformLocation(shaderProgram, "projection");
	//uModelview = glGetUniformLocation(shaderProgram, "modelview");
	////uLightColor = glGetUniformLocation(shaderProgram, "lightColor");
	//// Now send these values to the shader program
	//glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	//glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	////glUniform3fv(uLightColor, 1, &lightColor[0]);
	//// Now draw the OBJObject. We simply need to bind the VAO associated with it.
	//glBindVertexArray(VAO);
	//// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	//glBindVertexArray(0);
}

void SpotLight::update()
{
	//spin(1.0f);
}