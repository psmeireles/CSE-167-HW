#include "window.h"

const char* window_title = "GLFW Starter Project";
Geometry *torso, *wheel, *arm, *eye;
Transform *robot,
	*torsoRot180x,
	*wheelRot90x, *wheelRot90y, *wheelRot90z,
	*wheelTNegZ, *wheelTPosZ, *wheelTNegX, *wheelTPosX, *wheelTNegY, *wheelTPosY, *scaleWheel,
	*neckTPosY, *scaleNeck,
	*eyeTPosY, *eyeTNegX, *eyeTPosX, *eyeRot90x, *scaleEye,
	*armRot90x, *armTNegX, *armTPosX, *armTPosZ, *scaleArm,
	*world;
Transform *armyT[100];
GLint objShader, lightShader;

// Default camera parameters
glm::vec3 Window::camPos(0.0f, 10.0f, 30.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

bool Window::movement = false;
bool toggleModel = true;
int Window::normalColor = 0;
int eyeDir = 0;
int signal = 1;
bool first = true;

glm::vec3 Window::lastPoint;
glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	glm::mat4 r90x = glm::rotate(glm::mat4(1.0f), 3.14f / 2.0f, glm::vec3(1.0, 0.0, 0.0)),
		r90y = glm::rotate(glm::mat4(1.0f), 3.14f / 2.0f, glm::vec3(0.0, 1.0, 0.0)),
		r90z = glm::rotate(glm::mat4(1.0f), 3.14f / 2.0f, glm::vec3(0.0, 0.0, 1.0)),
		mTNegZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0)),
		mTPosZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 1.0)),
		mTNegX = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 0.0, 0.0)),
		mTPosX = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, 0.0)),
		mTNegY = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -1.0, 0.0)),
		mTPosY = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.0, 0.0));

	robot = new Transform(glm::mat4(1.0f));
	torso = new Geometry("../obj/body_s.obj");
	eye = new Geometry("../obj/body_s.obj");
	wheel = new Geometry("../obj/eyeball_s.obj");
	arm = new Geometry("../obj/limb_s.obj");

	torsoRot180x = new Transform(r90x*r90x);

	wheelRot90x = new Transform(r90x);
	wheelRot90y = new Transform(r90y);
	wheelRot90z = new Transform(r90z);
	wheelTNegZ = new Transform(mTNegZ);
	wheelTPosZ = new Transform(mTPosZ);
	wheelTNegX = new Transform(mTNegX);
	wheelTPosX = new Transform(mTPosX);
	wheelTNegY = new Transform(mTNegY);
	wheelTPosY = new Transform(mTPosY);
	scaleWheel = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.3)));

	neckTPosY = new Transform(mTPosY);
	scaleNeck = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 1.0, 0.2)));

	eyeTNegX = new Transform(glm::translate(mTNegX, glm::vec3(0.6, 0.0, 0.0)));
	eyeTPosX = new Transform(glm::translate(mTPosX, glm::vec3(-0.6, 0.0, 0.0)));
	eyeTPosY = new Transform(glm::translate(mTNegZ, glm::vec3(0.0, 0.0, -0.2)));
	eyeRot90x = new Transform(r90x);
	scaleEye = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.5, 0.4)));

	armTNegX = new Transform(mTNegX);
	armTPosX = new Transform(mTPosX);
	armTPosZ = new Transform(glm::translate(mTPosY, glm::vec3(0.0, -0.5, 0.0)));
	armRot90x = new Transform(r90x);
	scaleArm = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 1.0, 0.5)));

	world = new Transform(glm::mat4(1.0f));

	// body
	robot->addChild(torsoRot180x);
	torsoRot180x->addChild(torso);

	// wheels
	robot->addChild(wheelRot90x);
	wheelRot90x->addChild(wheelTNegX);
	wheelRot90x->addChild(wheelTPosX);
	wheelTNegX->addChild(wheelTPosZ);	// left wheels
	wheelTPosX->addChild(wheelTPosZ);	// right wheels
	wheelTPosZ->addChild(scaleWheel);
	wheelTPosZ->addChild(wheelTPosY);
	wheelTPosZ->addChild(wheelTNegY);
	wheelTNegY->addChild(scaleWheel);
	wheelTPosY->addChild(scaleWheel);
	scaleWheel->addChild(wheel);

	// neck
	robot->addChild(neckTPosY);
	neckTPosY->addChild(scaleNeck);
	scaleNeck->addChild(torso);

	// eyes
	neckTPosY->addChild(eyeRot90x);
	eyeRot90x->addChild(eyeTPosY);
	eyeTPosY->addChild(eyeTNegX);
	eyeTPosY->addChild(eyeTPosX);
	eyeTNegX->addChild(scaleEye);
	eyeTPosX->addChild(scaleEye);
	scaleEye->addChild(eye);

	// arms
	robot->addChild(armRot90x);
	armRot90x->addChild(armTPosZ);
	armTPosZ->addChild(armTNegX);
	armTPosZ->addChild(armTPosX);
	armTNegX->addChild(scaleArm);
	armTPosX->addChild(scaleArm);
	scaleArm->addChild(arm);

	for (int i = 0; i < 100; i++) {
		armyT[i] = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(5.0*(i % 10) - 25.0, 0.0, 5.0*(i / 10) - 25.0)));
		armyT[i]->addChild(robot);
		world->addChild(armyT[i]);
	}

	// Load the shader program. Make sure you have the correct filepath up top
	objShader = LoadShaders("../shader.vert", "../shader.frag");
	lightShader = LoadShaders("../lightShader.vert", "../lightShader.frag");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(torso);
	delete(arm);
	delete(wheel);
	delete(eye);
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
		P = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(camPos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	wheel->toWorld = glm::rotate(wheel->toWorld, 0.1f, glm::vec3(1.0, 0.0, 0.0));

	float eyeRot = 0.02;
	eyeTPosY->M = glm::rotate(eyeTPosY->M, eyeRot*signal, glm::vec3(0.0, 0.0, 1.0));
	neckTPosY->M = glm::translate(neckTPosY->M, glm::vec3(0.0, eyeRot*signal, 0.0));
	eyeDir++;
	if (eyeDir == 40 && first) {
		eyeDir = 0;
		signal = -signal;
		first = false;
	}
	else if (eyeDir == 80) {
		eyeDir = 0;
		signal = -signal;
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	world->draw(objShader, Window::V);	

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
		case GLFW_KEY_N:
			Window::normalColor = (Window::normalColor + 1) % 2;
			printf("%d\n", normalColor);
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_P:
			break;
		case GLFW_KEY_0:
			toggleModel = !toggleModel;
			break;
		case GLFW_KEY_1:
			break;
		case GLFW_KEY_2:
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
		world->scale(yoffset > 0.0f ? 1.1 : 0.9);
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
				world->M = rotMatrix * world->M;
			}
			else {
				Window::V = rotMatrix * Window::V;
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
	v.z = camPos.z;
	v.x /= v.length();
	v.y /= v.length();
	v.z /= v.length();
	return v;
}