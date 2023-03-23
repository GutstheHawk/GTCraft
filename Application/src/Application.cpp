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
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "GL_Debug.h";
#include "Camera.h"
#include "UserInputs.h"
#include "Texture.h"

static std::string ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;
	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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
	VertexBuffer skyboxVB(positions, 3 * 36 * sizeof(float));

	VertexBufferLayout skyboxLayout;
	skyboxLayout.Push<float>(3, GL_FALSE);

	VertexArray skyboxVA;
	skyboxVA.AddBuffer(skyboxVB, skyboxLayout);
	skyboxVA.Bind();

	//Atlas vertex buffer, vertex array, and index buffer
	VertexBuffer atlasVB(positions, 5 * 24 * sizeof(float));

	VertexBufferLayout atlasLayout;
	atlasLayout.Push<float>(3, GL_FALSE);
	atlasLayout.Push<float>(2, GL_FALSE);

	VertexArray atlasVA;
	atlasVA.AddBuffer(atlasVB, atlasLayout);

	IndexBuffer ib(indices, 36);
	ib.Bind();


	//Atlas Shader Code
	std::string atlasVertexShader = ParseShader("res/shaders/atlasVertexShader.shader");
	std::string atlasFragmentShader = ParseShader("res/shaders/atlasFragmentShader.shader");
	unsigned int atlasShader = CreateShader(atlasVertexShader, atlasFragmentShader);
	glLinkProgram(atlasShader);
	glUseProgram(atlasShader);

	//Atlas Shader Code
	std::string skyboxVertexShader = ParseShader("res/shaders/skyboxVertexShader.shader");
	std::string skyboxFragmentShader = ParseShader("res/shaders/skyboxFragmentShader.shader");
	unsigned int skyboxShader = CreateShader(skyboxVertexShader, skyboxFragmentShader);
	glLinkProgram(skyboxShader);


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
	int texLocation = glGetUniformLocation(atlasShader, "u_Texture");
	if (texLocation == -1)
	{
		printf("Warning: uniform texLocation doesn't exist!\n");
	}
	printf("TexLocation: %d\n", texLocation);
	glUniform1i(texLocation, 0);

	int atlasOffsetSize = glGetUniformLocation(atlasShader, "v_atlasOffsetSize");
	glUniform2f(atlasOffsetSize, 16.0f, 16.0f);
	int atlasIndexes = glGetUniformLocation(atlasShader, "v_atlasIndexes");
	glUniform2f(atlasIndexes, 2.0f, 15.0f);

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
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

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

		glDepthMask(GL_FALSE);  // change depth function so depth test passes when values are equal to depth buffer's content
		glUseProgram(skyboxShader);
		glm::mat4 sbView = glm::mat4(glm::mat3(cam->ReturnUpdatedView())); // remove translation from the view matrix
		int location = glGetUniformLocation(skyboxShader, "projection");
		glUniformMatrix4fv(location, 1, GL_FALSE, &projection[0][0]);
		location = glGetUniformLocation(skyboxShader, "view");
		glUniformMatrix4fv(location, 1, GL_FALSE, &sbView[0][0]);
		// skybox cube
		skyboxVA.Bind();
		skyboxTexture.Bind(GL_TEXTURE_CUBE_MAP, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		skyboxVA.Unbind();
		glDepthMask(GL_TRUE);; // set depth function back to default

		glUseProgram(atlasShader);

		location = glGetUniformLocation(atlasShader, "projection");
		glUniformMatrix4fv(location, 1, GL_FALSE, &projection[0][0]);

		location = glGetUniformLocation(atlasShader, "view");
		glUniformMatrix4fv(location, 1, GL_FALSE, &view[0][0]);

		location = glGetUniformLocation(atlasShader, "model");
		//glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);

		atlasVA.Bind();
		for (unsigned int y = 0; y < 16; y++)
		{
			if (y > 14)
			{
				glUniform2f(atlasIndexes, 0.0f, 15.0f);
			}
			else if (y < 3)
			{
				glUniform2f(atlasIndexes, 1.0f, 15.0f);

			}
			else
			{
				glUniform2f(atlasIndexes, 2.0f, 15.0f);
			}
			for (unsigned int z = 0; z < 16; z++)
			{
				for (unsigned int x = 0; x < 16; x++)
				{
					cam->ModelTransform(glm::vec3(x * 2.0f, y * 2.0f, z * 2.0f));
					model = cam->ReturnModel();
					glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);
					glDrawElements(GL_TRIANGLES, size / sizeof(GLfloat), GL_UNSIGNED_INT, nullptr);
				}
			}
		}
		atlasVA.Unbind();
		//Resets Model matrix to world origin - Identity Matrix
		//cam->ResetModel();



		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(atlasShader);
	glDeleteProgram(skyboxShader);

	glfwTerminate();
	return 0;
}