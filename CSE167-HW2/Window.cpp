#include "window.h"

const char* window_title = "GLFW Starter Project";
OBJObject *bunny, *bear, *dragon, *currentOBJ;
PointLight *light;
SpotLight *spotlight;
GLint objShader, lightShader;
glm::vec3 Window::camPos;

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 10.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

bool Window::movement = false;
bool togglePointLight = true;
bool toggleSpotLight = true;
bool toggleModel = true;

glm::vec3 Window::lastPoint;
glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{

	bunny = new OBJObject("../../obj/bunny.obj", Material(glm::vec3(1.0f, 0.41f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f));
	bear = new OBJObject("../../obj/bear.obj", Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.41f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f));
	dragon = new OBJObject("../../obj/dragon.obj", Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.41f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f));
	currentOBJ = bunny;

	light = new PointLight();
	spotlight = new SpotLight();

	camPos = glm::vec3(0.0f, 0.0f, 10.0f);

	// Load the shader program. Make sure you have the correct filepath up top
	objShader = LoadShaders("../shader.vert", "../shader.frag");
	lightShader = LoadShaders("../lightShader.vert", "../lightShader.frag");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(bunny);
	delete(bear);
	delete(dragon);
	glDeleteProgram(objShader);
	glDeleteProgram(lightShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	currentOBJ->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the cube
	currentOBJ->draw(objShader);
	light->draw(lightShader);
	spotlight->draw(lightShader);
	

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		switch (key) {
		// Check if escape was pressed
		case(GLFW_KEY_ESCAPE):
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		case GLFW_KEY_F1:
			currentOBJ = bunny;
			break;
		case GLFW_KEY_F2:
			currentOBJ = bear;
			break;
		case GLFW_KEY_F3:
			currentOBJ = dragon;
			break;
		case GLFW_KEY_N:
			OBJObject::normalColor = (OBJObject::normalColor + 1) % 2;
			break;
		case GLFW_KEY_W:
			if (mods == GLFW_MOD_SHIFT) {
				SpotLight::coneAngle *= 1.1f;
			}
			else {
				SpotLight::coneAngle *= 0.9f;
			}
			SpotLight::cutOff = glm::cos(glm::radians(SpotLight::coneAngle));
			SpotLight::outerCutOff = glm::cos(glm::radians(SpotLight::coneAngle * SpotLight::outerCutOffMultiplier));
			break;
		case GLFW_KEY_E:
			if (mods == GLFW_MOD_SHIFT) {
				SpotLight::outerCutOffMultiplier *= 1.1f;
			}
			else {
				SpotLight::outerCutOffMultiplier *= 0.9f;
			}
			SpotLight::outerCutOff = glm::cos(glm::radians(SpotLight::coneAngle * SpotLight::outerCutOffMultiplier));
			break;
		case GLFW_KEY_S:
			SpotLight::currColor = SpotLight::currColor == glm::vec3(0.0f, 0.0f, 0.0f) ? SpotLight::actualColor : glm::vec3(0.0f, 0.0f, 0.0f);
			break;
		case GLFW_KEY_P:
			PointLight::currColor = PointLight::currColor == glm::vec3(0.0f, 0.0f, 0.0f) ? PointLight::actualColor : glm::vec3(0.0f, 0.0f, 0.0f);
			break;
		case GLFW_KEY_0:
			toggleModel = !toggleModel;
			break;
		case GLFW_KEY_1:
			togglePointLight = !togglePointLight;
			break;
		case GLFW_KEY_2:
			toggleSpotLight = !toggleSpotLight;
			break;
		}		
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			movement = true;

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			lastPoint = trackBallMapping(glm::vec2(xpos, ypos));
		}
		else if (action == GLFW_RELEASE) {
			movement = false;
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (toggleModel) {
		currentOBJ->scale(yoffset > 0.0f ? 1.1 : 0.9);
	}
	else {
		if (togglePointLight) {
			PointLight::lightPos.x -= PointLight::lightPos.x / PointLight::lightPos.length() * (yoffset > 0 ? 0.5f : -0.5f);
			PointLight::lightPos.y -= PointLight::lightPos.y / PointLight::lightPos.length() * (yoffset > 0 ? 0.5f : -0.5f);
			PointLight::lightPos.z -= PointLight::lightPos.z / PointLight::lightPos.length() * (yoffset > 0 ? 0.5f : -0.5f);
		}
		if (toggleSpotLight) {
			SpotLight::lightPos.x -= SpotLight::lightPos.x / SpotLight::lightPos.length() * (yoffset > 0 ? 0.5f : -0.5f);
			SpotLight::lightPos.y -= SpotLight::lightPos.y / SpotLight::lightPos.length() * (yoffset > 0 ? 0.5f : -0.5f);
			SpotLight::lightPos.z -= SpotLight::lightPos.z / SpotLight::lightPos.length() * (yoffset > 0 ? 0.5f : -0.5f);
		}
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec3 direction;
	float rot_angle;
	glm::vec3 curPoint;
	glm::vec2 point = glm::vec2(xpos, ypos);
	if (movement) {
		curPoint = trackBallMapping(point);
		direction = curPoint - lastPoint;
		float velocity = direction.length();

		if (velocity > 0.0001) {
			glm::vec3 rotAxis;
			rot_angle = 0.1;
			rotAxis = glm::cross(lastPoint, curPoint);
			glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), rot_angle, rotAxis);
			if (toggleModel) {
				currentOBJ->toWorld = rotMatrix * currentOBJ->toWorld;
			}
			if (togglePointLight) {
				glm::vec4 newPos = rotMatrix * glm::vec4(PointLight::lightPos.x, PointLight::lightPos.y, PointLight::lightPos.z, 1.0f);
				PointLight::lightPos = glm::vec3(newPos.x, newPos.y, newPos.z);
			}
			if (toggleSpotLight) {
				glm::vec4 newPos = rotMatrix * glm::vec4(SpotLight::lightPos.x, SpotLight::lightPos.y, SpotLight::lightPos.z, 1.0f);
				SpotLight::lightPos = glm::vec3(newPos.x, newPos.y, newPos.z);
				SpotLight::lightDirection = glm::normalize(-SpotLight::lightPos);
			}
		}

		lastPoint = curPoint;
	}
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;
	float d;
	v.x = (2.0*point.x - width) / width;
	v.y = (height - 2.0*point.y) / height;
	v.z = 0;
	d = v.length();
	d = (d < 1.0) ? d : sqrt(0.5);
	v.z = 1;
	v.x /= v.length();
	v.y /= v.length();
	v.z /= v.length();
	return v;
}