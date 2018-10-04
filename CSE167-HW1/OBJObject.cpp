#include "OBJObject.h"
#include <limits>

OBJObject::OBJObject(const char *filepath) 
{
	angle = 0.0f;
	toWorld = glm::mat4(1.0f);
	min_x = std::numeric_limits<float>::max();
	min_y = std::numeric_limits<float>::max();
	min_z = std::numeric_limits<float>::max();
	max_x = std::numeric_limits<float>::lowest();
	max_y = std::numeric_limits<float>::lowest();
	max_z = std::numeric_limits<float>::lowest();
	animationCounter = 0;
	parse(filepath);
}

void OBJObject::parse(const char *filepath) 
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	char c1, c2;
	GLfloat x, y, z, r, g, b, aux_x, aux_y, aux_z;

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
				this->aux.push_back(glm::vec3(x, y, z));
				updateMinMaxCoordinates(x, y, z);

				aux_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4.0f - 2.0f;
				aux_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4.0f - 2.0f;
				aux_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4.0f - 2.0f;
				this->vertices.push_back(glm::vec3(aux_x, aux_y, aux_z));
				this->steps.push_back(glm::vec3((x - aux_x) / 100.0f, (y - aux_y) / 100.0f, (z - aux_z) / 100.0f));
			}
		}
		c1 = fgetc(fp);
	}
	fclose(fp);

	shiftAndResizeModel();
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glColor3f(colors[i].x, colors[i].y, colors[i].z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update()
{
	if (animationCounter != 100) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].x += steps[i].x;
			vertices[i].y += steps[i].y;
			vertices[i].z += steps[i].z;
		}
		animationCounter++;
	}
	spin(1.0f);
}

void OBJObject::spin(float deg)
{
	this->angle += deg;

	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the OBJ
	this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
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

void OBJObject::changeColor(GLint option) {
	for (int i = 0; i < colors.size(); i++) {
		if (option == 0) {
			colors[i].x = normals[i].x;
			colors[i].y = normals[i].y;
			colors[i].z = normals[i].z;
		}
		else {
			colors[i].x = vertices[i].x;
			colors[i].y = vertices[i].y;
			colors[i].z = vertices[i].z;
		}
	}
}

void OBJObject::restartModel() {
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x -= steps[i].x * animationCounter;
		vertices[i].y -= steps[i].y * animationCounter;
		vertices[i].z -= steps[i].z * animationCounter;
	}
	animationCounter = 0;
}