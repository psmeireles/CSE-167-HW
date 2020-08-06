#include "Window.h"

const char* window_title = "GLFW Starter Project";
Cube cube(5.0f);
OBJObject bunny = OBJObject("../../obj/bunny.obj");
OBJObject bear = OBJObject("../../obj/bear.obj");
OBJObject dragon = OBJObject("../../obj/dragon.obj");
OBJObject *currentOBJ = &bunny;
float pointSize = 1.0f;
int Window::width;
int Window::height;

void Window::initialize_objects()
{
}

void Window::clean_up()
{
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1006);

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
	// Set the viewport size
	glViewport(0, 0, width, height);
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 2.6 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -2.6);
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the OBJ slightly.
	(*currentOBJ).update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();
	
	// Render objects
	(*currentOBJ).draw();

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
		// Check if escape was pressed
		switch (key) {
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		case GLFW_KEY_F1:
			(*currentOBJ).restartModel();
			currentOBJ = &bunny;
			break;
		case GLFW_KEY_F2:
			(*currentOBJ).restartModel();
			currentOBJ = &bear;
			break;
		case GLFW_KEY_F3:
			(*currentOBJ).restartModel();
			currentOBJ = &dragon;
			break;
		case GLFW_KEY_P:
			if (mods == GLFW_MOD_SHIFT) {
				pointSize++;
			}
			else {
				pointSize--;
			}
			glPointSize(pointSize);
			break;
		case GLFW_KEY_C:
			if (mods == GLFW_MOD_SHIFT) {
				(*currentOBJ).changeColor(0);
			}
			else {
				(*currentOBJ).changeColor(1);
			}
			break;
		}
	}
}
