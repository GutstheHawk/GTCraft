#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
//#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
//#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "GL_Debug.h";
#include "Camera.h"
#include "UserInputs.h"
#include "Texture.h"
#include "Shader.h"
#include "ChunkGeneration.h"
#include "Superchunk.h"

//static std::array<Vertex, 24> CreateCube(float x, float y, float z)
//{
//	Vertex v0;
//}

int main(void)
{

	/* Initialize the library */
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW.");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const int windowWidth = 1280;
	const int windowHeight = 720;
	const char* windowTitle = "GALINT: MC Clone";

	GLFWwindow* window;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(
		windowWidth,
		windowHeight,
		windowTitle,
		nullptr,
		nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW.");
		return -1;
	}

	glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, NULL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float positions[] = {
		//front
		-1.0, -1.0,  1.0, 0.0, 0.0, //0 bottom-left
		 1.0, -1.0,  1.0, 1.0, 0.0, //1 bottom-right
		 1.0,  1.0,  1.0, 1.0, 1.0, //2 top-right
		-1.0,  1.0,  1.0, 0.0, 1.0, //3 top-left
		//top
		-1.0,  1.0,  1.0, 0.0, 0.0, //4
		 1.0,  1.0,  1.0, 1.0, 0.0, //5
		 1.0,  1.0, -1.0, 1.0, 1.0, //6
		-1.0,  1.0, -1.0, 0.0, 1.0, //7
		//back
		 1.0, -1.0, -1.0, 0.0, 0.0, //8
		-1.0, -1.0, -1.0, 1.0, 0.0, //9
		-1.0,  1.0, -1.0, 1.0, 1.0, //10
		 1.0,  1.0, -1.0, 0.0, 1.0, //11
		//bottom
		-1.0, -1.0, -1.0, 0.0, 0.0, //12
		 1.0, -1.0, -1.0, 1.0, 0.0, //13
		 1.0, -1.0,  1.0, 1.0, 1.0, //14
		-1.0, -1.0,  1.0, 0.0, 1.0, //15
		//left
		-1.0, -1.0, -1.0, 0.0, 0.0, //16
		-1.0, -1.0,  1.0, 1.0, 0.0, //17
		-1.0,  1.0,  1.0, 1.0, 1.0, //18
		-1.0,  1.0, -1.0, 0.0, 1.0, //19
		//right
		 1.0, -1.0,  1.0, 0.0, 0.0, //20
		 1.0, -1.0, -1.0, 1.0, 0.0, //21
		 1.0,  1.0, -1.0, 1.0, 1.0, //22
		 1.0,  1.0,  1.0, 0.0, 1.0  //23

	};

	unsigned int indices[] = {
		//front
		1,  2,  3,
		3,  0,  1,
		//top
		4,  5,  6,
		6,  7,  4,
		//back
		8,  9,  10,
		10, 11, 8,
		//bottom
		12, 13, 14,
		14, 15, 12,
		//left
		16, 17, 18,
		18, 19, 16,
		//right
		20, 21, 22,
		22, 23, 20

	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	////Skybox vertex buffer, vertex array, and index buffer
	VertexBuffer skyboxVB(positions, 3 * 36 * sizeof(float), GL_STATIC_DRAW);

	VertexBufferLayout skyboxLayout;
	skyboxLayout.Push<float>(3, GL_FALSE);

	VertexArray skyboxVA;
	skyboxVA.AddBuffer(skyboxVB, skyboxLayout);
	skyboxVA.Bind();

	//Atlas vertex buffer, vertex array, and index buffer GL_DYNAMIC_DRAW 24 * 16 * 16 * 16 * sizeof(Vertex)
	/*VertexBuffer atlasVB(nullptr, 24 * 16 * 16 * 16 * sizeof(Vertex), GL_DYNAMIC_DRAW);

	VertexBufferLayout atlasLayout;
	atlasLayout.Push<float>(3, GL_FALSE);
	atlasLayout.Push<float>(2, GL_FALSE);

	VertexArray atlasVA;
	atlasVA.AddBuffer(atlasVB, atlasLayout);

	IndexBuffer ib(indices, 36);
	ib.Bind();*/


	//Atlas Shader Code
	Shader atlasShader("res/shaders/chunkVertexShader.shader", "res/shaders/chunkFragmentShader.shader");
	atlasShader.Bind();
	//glLinkProgram(atlasShader);

	//Atlas Shader Code
	Shader skyboxShader("res/shaders/skyboxVertexShader.shader", "res/shaders/skyboxFragmentShader.shader");
	//glLinkProgram(skyboxShader);

	//Setup camera and player controls
	Camera* cam = new Camera();
	PlayerControls* pc = new PlayerControls(cam);
	glfwSetWindowUserPointer(window, pc);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Texture texture("res/textures/dirt.png");
	Texture texture("res/textures/alternate_atlas.png", 15, 15, 15, 15);
	texture.Bind(GL_TEXTURE_2D);
	atlasShader.SetUniform1i("u_Texture", 0);

	//atlasShader.SetUniform2f("v_atlasOffsetSize", 16.0f, 16.0f);
	//atlasShader.SetUniform2f("v_atlasIndexes", 2.0f, 15.0f);

	//Skybox
	std::vector<std::string> faces
	{
			"res/textures/skybox/skybox_right.png",
			"res/textures/skybox/skybox_left.png",
			"res/textures/skybox/skybox_top.png",
			"res/textures/skybox/skybox_bottom.png",
			"res/textures/skybox/skybox_front.png",
			"res/textures/skybox/skybox_back.png"
	};

	Texture skyboxTexture(0);
	skyboxTexture.loadCubemap(faces);

	//Time
	float deltaTime = 0.0f;
	float currentFrame = 0.0f;
	float lastFrame = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	int size;  
	//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	//Block* chunk = generateChunk();
	//printChunk(chunk);
	
	Superchunk sChunk;
	//sChunk.fillSuperchunk();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/*Render here */
		//glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cam->SetCameraSpeed(deltaTime);

		pc->ProcessInputs();

		//glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 projection = cam->ReturnProjection();
		glm::mat4 view = cam->ReturnView();
		glm::mat4 model = cam->ReturnModel();

		//atlasShader.Unbind();

		//glDepthMask(GL_FALSE);  // change depth function so depth test passes when values are equal to depth buffer's content
		//skyboxShader.Bind();
		////glUseProgram(skyboxShader);
		//glm::mat4 sbView = glm::mat4(glm::mat3(cam->ReturnUpdatedView())); // remove translation from the view matrix
		//skyboxShader.SetUniformMatrix4fv("projection", 1, GL_FALSE, &projection[0][0]);
		//skyboxShader.SetUniformMatrix4fv("view", 1, GL_FALSE, &sbView[0][0]);
		//// skybox cube
		//skyboxVA.Bind();
		//skyboxTexture.Bind(GL_TEXTURE_CUBE_MAP, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//skyboxVA.Unbind();
		//glDepthMask(GL_TRUE);; // set depth function back to default
		//skyboxShader.Unbind();

		//atlasShader.Bind();
		/*atlasShader.SetUniformMatrix4fv("projection", 1, GL_FALSE, &projection[0][0]);
		atlasShader.SetUniformMatrix4fv("view", 1, GL_FALSE, &view[0][0]);
		atlasShader.SetUniformMatrix4fv("model", 1, GL_FALSE, &model[0][0]);*/

		glm::mat4 pvm = projection * view * model;
		//atlasShader.SetUniformMatrix4fv("pvm", 1, GL_FALSE, &pvm[0][0]);

		//startingChunk.render();

		//atlasVA.Bind();

		sChunk.render(cam, &atlasShader);

		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		/*for (unsigned int y = 0; y < 16; y++)
		{
			if (y > 14)
			{
				atlasShader.SetUniform2f("v_atlasIndexes", 0.0f, 15.0f);
			}
			else if (y < 3)
			{
				atlasShader.SetUniform2f("v_atlasIndexes", 1.0f, 15.0f);
			}
			else
			{
				atlasShader.SetUniform2f("v_atlasIndexes", 2.0f, 15.0f);
			}
			for (unsigned int z = 0; z < 16; z++)
			{
				for (unsigned int x = 0; x < 16; x++)
				{
					cam->ModelTransform(glm::vec3(x * 2.0f, y * 2.0f, z * 2.0f));
					model = cam->ReturnModel();
					atlasShader.SetUniformMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
					glDrawElements(GL_TRIANGLES, size / sizeof(GLfloat), GL_UNSIGNED_INT, nullptr);
				}
			}
		}*/
		//atlasVA.Unbind();
		//Resets Model matrix to world origin - Identity Matrix
		//cam->ResetModel();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}