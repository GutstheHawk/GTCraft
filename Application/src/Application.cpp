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
		//front face
		-0.5f, -0.5f, 0.0f, 0.0f, //0
		 0.5f, -0.5f, 1.0f, 0.0f, //1
		 0.5f,  0.5f, 1.0f, 1.0f, //2
		-0.5f,  0.5f, 0.0f, 1.0f  //3

	};

	unsigned int indices[] = {
		//front face
		0, 1, 2,
		2, 3, 0

	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2, GL_FALSE);
	layout.Push<float>(2, GL_FALSE);

	VertexArray va;
	va.AddBuffer(vb, layout);
	va.Bind();

	IndexBuffer ib(indices, 6);
	ib.Bind();

	//Shader Code
	std::string vertexShader = ParseShader("res/shaders/vertexShader.shader");
	std::string fragmentShader = ParseShader("res/shaders/fragmentShader.shader");
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glLinkProgram(shader);
	glUseProgram(shader);

	Camera* cam = new Camera();
	PlayerControls* pc = new PlayerControls(cam);
	glfwSetWindowUserPointer(window, pc);

	glfwSetKeyCallback(window, keyCallback);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	Texture texture("res/textures/dirtfront.png");
	texture.Bind();
	int texLocation = glGetUniformLocation(shader, "u_Texture");
	if (texLocation == -1)
	{
		printf("Warning: uniform texLocation doesn't exist!\n");
	}
	printf("TexLocation: %d\n", texLocation);
	glUniform1i(texLocation, 0);

	//Time
	float deltaTime = 0.0f;
	float currentFrame = 0.0f;
	float lastFrame = 0.0f;

	//glEnable(GL_DEPTH_TEST);

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

		glm::mat4 projection = cam->ReturnProjection();
		glm::mat4 view = cam->ReturnView();
		glm::mat4 model = cam->ReturnModel();

		int location = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(location, 1, GL_FALSE, &projection[0][0]);

		location = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(location, 1, GL_FALSE, &view[0][0]);

		location = glGetUniformLocation(shader, "model");
		glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);

		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}