#include <GL/glew.h>		// include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

#include "InitShader.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>

#define MESH_FILE "detailed_pig.obj"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

//using namespace glm;
//using namespace std;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
int g_gl_width = 640;
int g_gl_height = 480;
unsigned short key_track = 0;
bool isPressed = false;
bool celmode = false;
double oldX, oldY;
glm::mat4  model_view;
glm::mat4  projection_model;

glm::vec4 points[NumVertices];
glm::vec2 texcoords[NumVertices];
glm::vec3   normals[NumVertices];
glm::vec4 curr_light_pos = glm::vec4(1.0, -2.0, 0.0, 1.0);
std::vector<GLfloat*> vertices;
std::vector<GLint> faces;

GLuint vao;
GLuint shader_programme, shader_programme_smooth, curr_programme;

GLuint AmbientProduct;
GLuint DiffuseProduct;
GLuint SpecularProduct;
GLuint LightPosition;
GLuint Shininess;

GLuint  ModelView, Projection;

glm::vec3 ls = glm::vec3(1.0, 1.0, 1.0);
GLfloat fov = 45.0, aspect = 1.0;
GLfloat  zNear = 0.1f, zFar = 100.0f;
GLuint  projection; // projection matrix uniform shader variable location
GLfloat s = 0.0;
glm::mat4 m(1.0);
bool lbutton_down = false;
GLfloat xcenter, ycenter, zcenter;
GLfloat scale;
double x_pre, y_pre, x, y;
float hrotate = 0;
float vrotate = 0;
double newX, newY;

GLfloat x_trans = 0, y_trans = 0, z_trans = 0;
// Vertices of a unit cube centered at origin, sides aligned with axes

int readfile(std::string addrstr)
{
	FILE *file1, *file2;
	file1 = fopen(addrstr.c_str(), "r"); //read
	if (file1 == NULL)
	{
		printf("File not found.\n");
		getchar();
	}

	float a, b, c, *arrayfloat, *arraycenter, *arrayx, *arrayy, *arrayz;
	GLint e, f, g, *arrayint;
	GLfloat min_x = FLT_MAX;	GLfloat min_y = FLT_MAX;	GLfloat min_z = FLT_MAX;
	GLfloat max_x = -10000.0;	GLfloat max_y = -10000.0;	GLfloat max_z = -10000.0;

	char v;
	int count = 0;

	while (!feof(file1))
	{
		v = fgetc(file1);
		if (v == 'v')
		{
			arrayfloat = new GLfloat[3];
			fscanf(file1, "%f%f%f", &a, &b, &c);
			arrayfloat[0] = a;
			arrayfloat[1] = b;
			arrayfloat[2] = c;
			//cout << a << " " << b << " " << c << endl;
			min_x = std::min(a, min_x);
			min_y = std::min(b, min_y);
			min_z = std::min(c, min_z);
			max_x = std::max(a, max_x);
			max_y = std::max(b, max_y);
			max_z = std::max(c, max_z);
			vertices.push_back(arrayfloat);

		}
		else if (v == 'f')
		{
			arrayint = new GLint[3];
			fscanf(file1, "%d%d%d", &e, &f, &g);
			faces.push_back(e);
			faces.push_back(f);
			faces.push_back(g);

		}
	}
	fclose(file1);

	xcenter = (max_x + min_x) / 2;
	ycenter = (max_y + min_y) / 2;
	zcenter = (max_z + min_z) / 2;

	scale = std::max(max_x - min_x, std::max(max_y - min_y, max_z - min_z));

	GLfloat *points;
	points = (GLfloat *)malloc(faces.size() * 3 * sizeof(GLfloat));
	GLfloat *normals;
	normals = (GLfloat *)malloc(faces.size() * 3 * sizeof(GLfloat));



	for (int i = 0; i < faces.size(); ++i) {
		points[i * 3] = ((GLfloat)vertices[faces[i] - 1][0]-xcenter)/scale;
		points[i * 3 + 1] = ((GLfloat)vertices[faces[i] - 1][1]-ycenter)/scale;
		points[i * 3 + 2] = ((GLfloat)vertices[faces[i] - 1][2]-zcenter)/scale;

	}
	for (int i = 0; i < faces.size(); i += 3) {
		GLint e = faces[i];
		GLint f = faces[i + 1];
		GLint g = faces[i + 2];
		glm::vec3 v1 = glm::vec3(vertices[e - 1][0], vertices[e - 1][1], vertices[e - 1][2]);
		glm::vec3 v2 = glm::vec3(vertices[f - 1][0], vertices[f - 1][1], vertices[f - 1][2]);
		glm::vec3 v3 = glm::vec3(vertices[g - 1][0], vertices[g - 1][1], vertices[g - 1][2]);
		glm::vec3 n = glm::triangleNormal(v1, v2, v3);
		normals[count] = n.x; count++;
		normals[count] = n.y; count++;
		normals[count] = n.z; count++;
		normals[count] = n.x; count++;
		normals[count] = n.y; count++;
		normals[count] = n.z; count++;
		normals[count] = n.x; count++;
		normals[count] = n.y; count++;
		normals[count] = n.z; count++;
	}
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * faces.size() * sizeof(GLfloat), points,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint normals_vbo;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * faces.size() * sizeof(GLfloat), normals,
		GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	


	return faces.size();
}

glm::vec3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	//gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	return glm::unProject(glm::vec3(winX, winY, winZ), model_view, projection_model, glm::vec4(1.0));

	/*
	(	detail::tvec3< T > const & 	win,
	detail::tmat4x4< T > const & 	model,
	detail::tmat4x4< T > const & 	proj,
	detail::tvec4< U > const & 	viewport
	)
	*/

	//return glm::vec4(posX, posY, posZ, 1.0);
}

void mymouse(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action) {
			isPressed = true;
			glfwGetCursorPos(window, &oldX, &oldY);
		}
		else if (GLFW_RELEASE == action) {
			isPressed = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (GLFW_PRESS == action)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			curr_light_pos = glm::vec4(GetOGLPos((int)x, (int)y), 1.0);
		}
	}
}

void key_handler(GLFWwindow* window, int button, int scancode, int action, int something)
{
	if (button == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		printf("space\n");
		if (key_track++ % 2 == 0)
		{
			curr_programme = shader_programme_smooth;
			celmode = true;
		}
		else
		{
			curr_programme = shader_programme;
			celmode = false;
		}

		ModelView = glGetUniformLocation(curr_programme, "ModelView");
		Projection = glGetUniformLocation(curr_programme, "Projection");

		AmbientProduct = glGetUniformLocation(curr_programme, "AmbientProduct");
		DiffuseProduct = glGetUniformLocation(curr_programme, "DiffuseProduct");
		SpecularProduct = glGetUniformLocation(curr_programme, "SpecularProduct");
		LightPosition = glGetUniformLocation(curr_programme, "LightPosition");
		Shininess = glGetUniformLocation(curr_programme, "Shininess");
	}
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("Callback\n");
	if (isPressed) {
		//double newX, newY;
		hrotate += 0.01*(xpos - oldX);// *drag_coeff;
		vrotate += 0.01*(ypos - oldY);

		glm::mat4 rot_view(1.0);
		rot_view = glm::rotate(rot_view, hrotate, glm::vec3(0, 0, 1));
		model_view = model_view*rot_view;

		glfwGetCursorPos(window, &oldX, &oldY);
	}
	else
	{
		hrotate = 0;
		vrotate = 0;
	}
}

int main() {

	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;
	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	/* We must specify 3.2 core if on Apple OS X -- other O/S can specify
	anything here. I defined 'APPLE' in the makefile for OS X

	Remove the #ifdef #endif and play around with this - you should be starting
	an explicit version anyway, and some non-Apple drivers will require this too.
	*/
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	window = glfwCreateWindow(g_gl_width, g_gl_height, "simple", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	/* start GLEW extension handler */
	glewExperimental = GL_TRUE;
	glewInit();
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION);	 /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);	// cull face
	glCullFace(GL_BACK);		 // cull back face
	glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front
	glClearColor(1, 1, 1, 1.0); // grey background to help spot mistakes
	
	int point_count = 0;
	point_count = readfile(MESH_FILE);

	/*-------------------------------CREATE
	* SHADERS-------------------------------*/
	shader_programme =
		InitShader("v_flat.glsl", "f_flat.glsl");
	shader_programme_smooth =
		InitShader("v_smooth.glsl", "f_smooth.glsl");
	GLuint outline_shader =
		InitShader("v_single.glsl", "f_single.glsl");
	//glUseProgram(shader_programme);
	curr_programme = shader_programme;

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(curr_programme, "ModelView");
	Projection = glGetUniformLocation(curr_programme, "Projection");
	
	AmbientProduct = glGetUniformLocation(curr_programme, "AmbientProduct");
    DiffuseProduct = glGetUniformLocation(curr_programme, "DiffuseProduct");
	SpecularProduct = glGetUniformLocation(curr_programme, "SpecularProduct");
	LightPosition = glGetUniformLocation(curr_programme, "LightPosition");
	Shininess = glGetUniformLocation(curr_programme, "Shininess");

	glfwSetMouseButtonCallback(window, mymouse);
	glfwSetKeyCallback(window, key_handler);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	/*
	uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
	uniform mat4 ModelView;
	uniform mat4 Projection;
	uniform vec4 LightPosition;
	uniform float Shininess;
	*/
	projection_model = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	glm::vec3  eye(0.0f, -2.0f, 0.5f);
	glm::vec3  at(0.0, 0.0, 0.0);
	glm::vec3  up(0.0, 1.0, 0.0);

	model_view = glm::lookAt(eye, at, up);
	while (!glfwWindowShouldClose(window)) {
		if (!isPressed)
		{
			hrotate = 0;
			vrotate = 0;
		}
		else
		{
			glfwGetCursorPos(window, &newX, &newY);
			if (newX == oldX && newY == oldY)
			{
				vrotate = 0;
				hrotate = 0;
			}
		}
		//model_view = glm::scale(model_view, glm::vec3(1.f/1.1, 1.f/1.1, 1.f/1.1));
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		// wipe the drawing surface clear
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer
		glStencilMask(0xFF); // enable writing to the stencil buffer

		glUseProgram(curr_programme);

		//camera
		/*
		glm::mat4 rot_view(1.0);
		rot_view = glm::rotate(rot_view, hrotate, glm::vec3(0, 0, 1));
		model_view = model_view*rot_view;
		*/
		glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
		glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projection_model));

		//LIGHTING
		glm::vec4 diffuse0 = glm::vec4(0, 1, 0, 1);
		glm::vec4 ambient0 = glm::vec4(1, 0, 0, 1.0);
		glm::vec4 specular0 = glm::vec4(0.2, 0.2, 0.2, 1.0);
		glm::vec4 light0_pos = curr_light_pos;

		glUniform4fv(DiffuseProduct, 1, glm::value_ptr(diffuse0));
		glUniform4fv(AmbientProduct, 1, glm::value_ptr(ambient0));
		glUniform4fv(SpecularProduct, 1, glm::value_ptr(specular0));
		glUniform4fv(LightPosition, 1, glm::value_ptr(light0_pos));
		glUniform1f(Shininess, 1.0f);
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, point_count);
		//glBindVertexArray(0);

		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glLineWidth(10);
		glPolygonMode(GL_FRONT, GL_LINE);
		glDisable(GL_DEPTH_TEST);

		glUseProgram(outline_shader);

		GLuint Proj = glGetUniformLocation(outline_shader, "Projection");
		GLuint Mod = glGetUniformLocation(outline_shader, "ModelView");

		//model_view = glm::scale(model_view, glm::vec3(1.1, 1.1, 1.1));
		glUniformMatrix4fv(Mod, 1, GL_FALSE, glm::value_ptr(model_view));
		glUniformMatrix4fv(Proj, 1, GL_FALSE, glm::value_ptr(projection_model));

		//glBindVertexArray(vao);
		if(celmode)
			glDrawArrays(GL_TRIANGLES, 0, point_count);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

		// update other events like input handling
		glfwPollEvents();
		//printf("Event loop\n");

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
