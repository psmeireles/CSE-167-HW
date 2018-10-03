#include "OBJObject.h"
#include <limits>
#include <math.h>
OBJObject::OBJObject(const char *filepath) 
{
	angle = 0.0f;
	toWorld = glm::mat4(1.0f);
	vertices.reserve(10000);
	indices.reserve(10000);
	normals.reserve(10000);
	min_x = std::numeric_limits<float>::max();
	min_y = std::numeric_limits<float>::max();
	min_z = std::numeric_limits<float>::max();
	max_x = std::numeric_limits<float>::lowest();
	max_y = std::numeric_limits<float>::lowest();
	max_z = std::numeric_limits<float>::lowest();

	parse(filepath);
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	char c1, c2;
	float x, y, z, r, g, b;
	int i = 0;
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
				fscanf(fp, " %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				this->vertices.push_back(glm::vec3(x, y, z));
				updateMinMaxCoordinates(x, y, z);
				i++;
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
		// Normalizing colors
		glColor3f((normals[i].x + 1) / 2, (normals[i].y + 1) / 2, (normals[i].z + 1) / 2);

		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update()
{
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
	float max_abs_x = abs(max_x) > abs(min_x) ? abs(max_x) : abs(min_x);
	float max_abs_y = abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y);
	float max_abs_z = abs(max_z) > abs(min_z) ? abs(max_z) : abs(min_z);
	float avg_x = (max_x + min_x) / 2;
	float avg_y = (max_y + min_y) / 2;
	float avg_z = (max_z + min_z) / 2;

	max_abs_x -= avg_x;
	max_abs_y -= avg_y;
	max_abs_z -= avg_z;

	float max_coord = max_abs_x;
	if (max_coord < max_abs_y) {
		max_coord = max_abs_y;
	}
	if (max_coord < max_abs_z) {
		max_coord = max_abs_z;
	}

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x -= avg_x;
		vertices[i].x /= max_coord/5;
		vertices[i].y -= avg_y;
		vertices[i].y /= max_coord/5;
		vertices[i].z -= avg_z;
		vertices[i].z /= max_coord/5;
	}
}