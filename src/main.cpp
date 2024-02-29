/*
ZJ Wood, Dunn, Eckhardt CPE 471 Lab base code
*/

#include <iostream>
#include <glad/glad.h>

#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
shared_ptr<Shape> shape;


double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}

int keyI, keyJ, keyK, keyL, keyM, keySpace = 0;

class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d;
	camera()
	{
		w = a = s = d = 0;
		pos = rot = glm::vec3(0, 0, 0);
	}
	glm::mat4 process(double ftime)
	{
		float speed = 0;
		if (w == 1)
		{
			speed = 1.5 * ftime;
		}
		else if (s == 1)
		{
			speed = -1.5 * ftime;
		}
		float yangle = 0;
		if (a == 1)
			yangle = -1.5 * ftime;
		else if (d == 1)
			yangle = 1.5 * ftime;
		rot.y += yangle;
		glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
		glm::vec4 dir = glm::vec4(0, 0, speed, 1);
		dir = dir * R;
		pos += glm::vec3(dir.x, dir.y, dir.z);
		glm::mat4 T = glm::translate(glm::mat4(1), pos);
		return R * T;
	}
};

camera mycam;

class snowflake {
	public:
		glm::vec3 relativePos;
		float rot;
		float minHeight;
		float maxHeight;
		float maxRadius;
		float randR;

		snowflake()
		{
			float rx = ((float)rand() / (RAND_MAX)) * 2 - 1;
			float rz = ((float)rand() / (RAND_MAX)) * 2 - 1;
			this->randR = ((float)rand() / (RAND_MAX)) * 6;


			this->minHeight = 0;
			this->maxHeight = 0;
			this->maxRadius = 0;

			this->relativePos = vec3(0, 0, 0);

			this->rot = 0.0f;

		}

		snowflake(float minH, float maxH, float maxR)
		{
			float rx = ((float)rand() / (RAND_MAX)) * 2 - 1;
			float ry = ((float)rand() / (RAND_MAX));
			float rz = ((float)rand() / (RAND_MAX)) * 2 - 1;
			this->randR = ((float)rand() / (RAND_MAX)) * 6;


			this->minHeight = minH;
			this->maxHeight = maxH;
			this->maxRadius = maxR;

			this->relativePos = vec3(rx * maxRadius, (maxHeight - minHeight) * ry + minHeight, rz * maxRadius);

			this->rot = 0.0f;

		}

		void iterate(double ftime) {
			this->relativePos.y = relativePos.y - 0.04 * ftime;
			this->rot = this->rot + 0.1 * ftime;
			if (relativePos.y < minHeight) {
				this->relativePos.y = maxHeight;
			}
		}

		vec3 getPos() {
			return vec3(vec4(relativePos, 1) * rotate(mat4(1.0f), rot, vec3(0, 1, 0)));
		}
};

snowflake snowflakes[50];

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> prog_noLighting;
	std::shared_ptr<Program> prog_tex;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our box to OpenGL
	GLuint VertexBufferID, VertexColorIDBox, IndexBufferIDBox;

	GLuint Texture;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			mycam.d = 0;
		}


		if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			keyI = 1;
		}
		if (key == GLFW_KEY_I && action == GLFW_RELEASE)
		{
			keyI = 0;
		}
		if (key == GLFW_KEY_J && action == GLFW_PRESS)
		{
			keyJ = 1;
		}
		if (key == GLFW_KEY_J && action == GLFW_RELEASE)
		{
			keyJ = 0;
		}
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
		{
			keyK = 1;
		}
		if (key == GLFW_KEY_K && action == GLFW_RELEASE)
		{
			keyK = 0;
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			keyL = 1;
		}
		if (key == GLFW_KEY_L && action == GLFW_RELEASE)
		{
			keyL = 0;
		}
		if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			keyM = 1;
		}
		if (key == GLFW_KEY_M && action == GLFW_RELEASE)
		{
			keyM = 0;
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			keySpace = 1;
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		{
			keySpace = 0;
		}

	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		/*
		double posX, posY;
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		*/
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		string resourceDirectory = "../resources" ;
		// Initialize mesh.
	
		//shape.loadMesh(resourceDirectory + "/t800.obj");
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize();
		shape->init();

		int width, height, channels;
		char filepath[1000];
		string str;

		str = resourceDirectory + "/feathers.jpg";

		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);





		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		glBindVertexArray(VertexArrayID);
		
		glGenBuffers(1, &VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		static const GLfloat g_vertex_buffer_data[] = {
			-0.7f, -0.3f, -0.3f,
			0.0f, -0.3f, 0.6f,
			0.7f, -0.3f, -0.3f,
			0.0f, 0.6f, 0.0f
		};

		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


		glGenBuffers(1, &IndexBufferIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		GLushort pyramid_elements[] = {

			// bottom
			0, 1, 2,
			//left
			0, 1, 3,
			//right
			1, 2, 3,
			//front
			0, 2, 3

		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_elements), pyramid_elements, GL_STATIC_DRAW);



		glBindVertexArray(0);


		//[TWOTEXTURES]
		//set the 2 textures to the correct samplers in the fragment shader:
		GLuint Tex1Location = glGetUniformLocation(prog_tex->pid, "tex");//tex, tex2... sampler in the fragment shader
		// Then bind the uniform samplers to texture units:
		glUseProgram(prog_tex->pid);
		glUniform1i(Tex1Location, 0);

	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		// enable blending and transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");

		prog->addUniform("setColor");
		prog->addUniform("campos");


		prog_noLighting = std::make_shared<Program>();
		prog_noLighting->setVerbose(true);
		prog_noLighting->setShaderNames(resourceDirectory + "/flat_vertShader.glsl", resourceDirectory + "/flat_fragShader.glsl");
		prog_noLighting->init();
		prog_noLighting->addUniform("P");
		prog_noLighting->addUniform("V");
		prog_noLighting->addUniform("M");
		prog_noLighting->addAttribute("vertPos");
		prog_noLighting->addAttribute("vertNor");
		prog_noLighting->addAttribute("vertTex");

		prog_noLighting->addUniform("setColor");
		prog_noLighting->addUniform("campos");


		prog_tex = std::make_shared<Program>();
		prog_tex->setVerbose(true);
		prog_tex->setShaderNames(resourceDirectory + "/tex_vertShader.glsl", resourceDirectory + "/tex_fragShader.glsl");
		prog_tex->init();
		prog_tex->addUniform("P");
		prog_tex->addUniform("V");
		prog_tex->addUniform("M");
		prog_tex->addAttribute("vertPos");
		prog_tex->addAttribute("vertNor");
		prog_tex->addAttribute("vertTex");

		prog_tex->addUniform("setColor");
		prog_tex->addUniform("campos");


		float minH = -0.7;
		float maxH = 0.55;
		float maxR = 0.4;
		for (size_t i = 0; i < sizeof(snowflakes) / sizeof(snowflakes[0]); i++)
		{
			snowflake flake = snowflakes[i];
			snowflakes[i] = snowflake(minH, maxH, maxR);
		}
		

	}


	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
		double frametime = get_last_elapsed_time();

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones
		V = mycam.process(frametime);
		
		vec4 setColor;
		

		//animation with the model matrix:
		static float w = 0.0;
		w += 1.0 * frametime;//rotation angle
		float trans = 0;// sin(t) * 2;
		//glm::mat4 RotateY = glm::rotate(glm::mat4(1), mycam.rot.y, glm::vec3(0, 1, 0));
		float angle = -3.1415926/2.0;

		// Draw the box using GLSL.
		
		//glUniform3fv(prog->getUniform("lightPos"), 1, &lightPos[0]);
		vec3 realCamPos = glm::rotate(glm::mat4(1), -1 * mycam.rot.y, glm::vec3(0, 1, 0)) * vec4(mycam.pos, 1);
		glUniform3fv(prog_noLighting->getUniform("campos"), 1, &mycam.pos[0]);

		glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 RotateY = glm::rotate(glm::mat4(1.0f), w, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 RotateZ = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), w, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		glm::mat4 Prev = mat4(1);
		glm::mat4 Shape = mat4(1);

		

		RotateX = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		RotateY = glm::rotate(glm::mat4(1.0f), 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		M = TransZ * RotateY * RotateX * S;


	

		static float flapPos = 0.8f * sin(w * 1.5f);
		static float flapPos2 = 0.5f * sin(w * 1.5f);

		static float leftWingPos = flapPos;
		static float rightWingPos = flapPos;
		static float leftWingPos2 = flapPos2;
		static float rightWingPos2 = flapPos2;
		static float headPos = flapPos * 0.03;
		static float tailPos = flapPos * 0.5;
		float bodyPos = -0.08f * sin(w * 1.5f);


		static int animating = 1;
		static int spaceRelease = 0;
		static int raiseLeftWing = 0;
		static int raiseRightWing = 0;
		static int raiseTail = 0;


		if (animating == 1 && keySpace && spaceRelease == 1) {
			animating = 0;
			spaceRelease = 0;
		}

		if (animating == 0 && keySpace && spaceRelease == 1) {
			animating = 1;
			spaceRelease = 0;
		}

		if (!keySpace) {
			spaceRelease = 1;
		}

		if (animating) {
			flapPos = 0.8f * sin(w * 1.5f);
			flapPos2 = 0.5f * sin(w * 1.5f);

			leftWingPos = flapPos;
			rightWingPos = flapPos;
			leftWingPos2 = flapPos2;
			rightWingPos2 = flapPos2;
			headPos = flapPos * 0.03;
			tailPos = flapPos * 0.5;
		}
		else {
			if (keyI) {
				headPos += 0.005;
			}
			if (keyK) {
				headPos -= 0.005;
			}
			if (headPos > 0.8 * 0.03) headPos = 0.8 * 0.03;
			if (headPos < -0.8 * 0.03) headPos = -0.8 * 0.03;


			if (keyM) {
				if (raiseTail) {
					tailPos += 0.03;
					if (tailPos >= 0.4) {
						raiseTail = 0;
					}
				}
				else {
					tailPos -= 0.03;
					if (tailPos <= -0.4) {
						raiseTail = 1;
					}
				}
			}

			if (keyJ) {
				if (raiseLeftWing) {
					leftWingPos += 0.05;
					leftWingPos2 += 0.03;
					if (leftWingPos >= 0.8) {
						raiseLeftWing = 0;
					}
				}
				else {
					leftWingPos -= 0.05;
					leftWingPos2 -= 0.03;
					if (leftWingPos <= -0.8) {
						raiseLeftWing = 1;
					}
				}
			}

			if (keyL) {
				if (raiseRightWing) {
					rightWingPos += 0.05;
					rightWingPos2 += 0.03;
					if (rightWingPos >= 0.8) {
						raiseRightWing = 0;
					}
				}
				else {
					rightWingPos -= 0.05;
					rightWingPos2 -= 0.03;
					if (rightWingPos <= -0.8) {
						raiseRightWing = 1;
					}
				}
			}
		}


		

		//BIRD BODY
		
		prog_tex->bind();

		setColor = vec4(0.8, 0.8, 1, 1);
		RotateY = glm::rotate(glm::mat4(1.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, bodyPos, 0.0f));
		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.09f, 0.09f, 0.18f));
		M = T * TransZ * RotateY * S;
		Prev = T * TransZ * RotateY;
		//send the matrices to the shaders
		glUniformMatrix4fv(prog_tex->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_tex->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_tex->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform3fv(prog_tex->getUniform("campos"), 1, &mycam.pos[0]);
		glUniform4fv(prog_tex->getUniform("setColor"), 1, &setColor[0]);

		shape->draw(prog_tex, FALSE);
		
		prog_tex->unbind();


		prog_noLighting->bind();

		//BIRD WINGS

		Shape = glm::rotate(glm::mat4(1.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * 
				glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.05)) * 
				glm::scale(glm::mat4(1.0f), glm::vec3(0.17f, 0.08f, 0.35f));

		setColor = vec4(1, 1, 1, 1);

		

		// WING ONE

		RotateZ = glm::rotate(glm::mat4(1.0f), leftWingPos, glm::vec3(0.0f, 0.0f, 1.0f));
		M = Prev * RotateZ * Shape;

		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);

		mat4 Prev2 = Prev * RotateZ;



		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f));

		T = glm::translate(glm::mat4(1.0f), glm::vec3(0.13f, 0.0f, 0.0f));
		RotateZ = glm::rotate(glm::mat4(1.0f), leftWingPos2, glm::vec3(0.0f, 0.0f, 1.0f));
		M = Prev2 * T * RotateZ * Shape * S;

		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);




		// WING TWO

		RotateY = glm::rotate(glm::mat4(1.0f), 3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
		RotateZ = glm::rotate(glm::mat4(1.0f), rightWingPos, glm::vec3(0.0f, 0.0f, 1.0f));
		M = Prev * RotateY * RotateZ * Shape;

		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);

		Prev2 = Prev * RotateY * RotateZ;



		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f));

		
		T = glm::translate(glm::mat4(1.0f), glm::vec3(0.13f, 0.0f, 0.0f));
		RotateZ = glm::rotate(glm::mat4(1.0f), rightWingPos2, glm::vec3(0.0f, 0.0f, 1.0f));
		M = Prev2 * T * RotateZ * Shape * S;

		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);



		// HEAD

		Shape = glm::rotate(glm::mat4(1.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.05)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.13f, 0.09f, 0.21f));

		T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, headPos + 0.04, 0.20f));
		RotateY = glm::rotate(glm::mat4(1.0f), 3.14159f * 0.7f, glm::vec3(0.0f, 1.0f, 0.0f));
		RotateZ = glm::rotate(glm::mat4(1.0f), 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		M = Prev * T * RotateY * RotateZ * Shape;

		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);
		


		//TAIL



		Shape = glm::rotate(glm::mat4(1.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.05 + 0.17f)) *
			glm::rotate(glm::mat4(1.0f), 3.14159f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), 0.8f * glm::vec3(0.17f, 0.08f, 0.35f));

		T = glm::translate(glm::mat4(1.0f), glm::vec3(-0.07f, 0.0f, 0.0f));
		RotateY = glm::rotate(glm::mat4(1.0f), 3.14159f * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		RotateZ = glm::rotate(glm::mat4(1.0f), tailPos + 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		M = T * Prev * RotateY * RotateZ * Shape;

		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);





		//SNOW ON GROUND
		RotateX = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		RotateY = glm::rotate(glm::mat4(1.0f), 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.2f, 0.6f));
		T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.7f, 0.0f));
		M = T * TransZ * RotateY * RotateX * S;

		setColor = vec4(1, 1, 1, 1);
		glUniformMatrix4fv(prog_noLighting->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

		shape->draw(prog_noLighting, FALSE);


		//SNOWFLAKES
		setColor = vec4(1, 1, 1, 1);
		TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));

		for (size_t i = 0; i < sizeof(snowflakes)/sizeof(snowflakes[0]); i++)
		{
			snowflakes[i].iterate(frametime);

			T = glm::translate(glm::mat4(1.0f), snowflakes[i].getPos());
			R = glm::rotate(glm::mat4(1.0f), snowflakes[i].randR + w*2, glm::vec3(1.0f, 1.0f, 0.0f));
			M = TransZ * T * S * R;
			glUniformMatrix4fv(prog_noLighting->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			glUniform4fv(prog_noLighting->getUniform("setColor"), 1, &setColor[0]);

			glBindVertexArray(VertexArrayID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);
		}


		prog_noLighting->unbind();



		
		prog->bind();

		//TRANSPARENT OBJECTS
		glDepthMask(false);


		setColor = vec4(0.9, 0.9, 0.9, 0.2);
		RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.75f));
		M = TransZ * RotateY * RotateX * S;
		//send the matrices to the shaders
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog->getUniform("setColor"), 1, &setColor[0]);

		shape->draw(prog, FALSE);

		setColor = vec4(0.9, 0.9, 1, 0.2);
		RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		S = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
		M = TransZ * RotateY * RotateX * S;
		//send the matrices to the shaders
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform4fv(prog->getUniform("setColor"), 1, &setColor[0]);
		
		shape->draw(prog, FALSE);	

		glDepthMask(true);
		
		prog->unbind();

		

		glBindVertexArray(0);
	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
