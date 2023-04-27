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
#include "Inventory.h"

#include "imgui/imgui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


//static std::array<Vertex, 24> CreateCube(float x, float y, float z)
//{
//	Vertex v0;
//}

#pragma pack(push, 1)
struct UIVertex
{
	glm::vec2 positions;
	glm::uvec2 texCoord_Type;
};
#pragma pack(pop)

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

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	const int windowWidth = 1280;
	const int windowHeight = 720;
	//const int windowWidth = 1920;
	//const int windowHeight = 1080;

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

	float skyboxVertices[] = {         
		// Front face
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
	VertexBuffer skyboxVB(skyboxVertices, 3 * 36 * sizeof(float), GL_STATIC_DRAW);

	VertexBufferLayout skyboxLayout;
	skyboxLayout.Push<float>(3, GL_FALSE);

	VertexArray skyboxVA;
	skyboxVA.AddBuffer(skyboxVB, skyboxLayout);
	skyboxVA.Bind();
	skyboxVA.Unbind();

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

	Shader rayShader("res/shaders/RayVertex.shader", "res/shaders/RayFragment.shader");

	//Setup camera and player controls
	Camera* cam = new Camera();
	cam->SetRayShader(&rayShader);
	cam->SetWorldShader(&atlasShader);
	glm::vec3 mapCenter = glm::vec3((SCX * 16) / 2, 3 * 16, (SCZ * 16) / 2);
	cam->Teleport(mapCenter);

	Superchunk* sChunk = new Superchunk;
	PlayerControls* pc = new PlayerControls(cam, sChunk);
	glfwSetWindowUserPointer(window, pc);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Texture texture("res/textures/dirt.png");
	Texture texture("res/textures/alternate_atlas.png");
	texture.Bind(GL_TEXTURE_2D, 0);
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

	Shader uiShader("res/shaders/uiVertexShader.shader", "res/shaders/uiFragmentShader.shader");

	UIVertex* uiVertices = new UIVertex[6];

	float blockUIWidth = (-1.0f + (1.0f / 4.0f));
	float blockUIHeight = (-1.0f + (1.0f / 4.0f));

	uiVertices[0] = UIVertex{ glm::vec2{-1.0f, -1.0f},				 glm::uvec2{0, pc->selectedBlockType} };
	uiVertices[1] = UIVertex{ glm::vec2{-1.0f, blockUIHeight},		 glm::uvec2{3, pc->selectedBlockType} };
	uiVertices[2] = UIVertex{ glm::vec2{blockUIWidth, -1.0f},		 glm::uvec2{1, pc->selectedBlockType} };
	uiVertices[3] = UIVertex{ glm::vec2{blockUIWidth, -1.0f},		 glm::uvec2{1, pc->selectedBlockType} };
	uiVertices[4] = UIVertex{ glm::vec2{-1.0f, blockUIHeight},		 glm::uvec2{3, pc->selectedBlockType} };
	uiVertices[5] = UIVertex{ glm::vec2{blockUIWidth,blockUIHeight}, glm::uvec2{2, pc->selectedBlockType} };

	VertexBuffer uiVB(uiVertices, 6 * sizeof(UIVertex), GL_STATIC_DRAW);

	VertexBufferLayout uiLayout;
	uiLayout.Push<float>(2, GL_FALSE);
	uiLayout.Push<unsigned int>(2, GL_FALSE);

	VertexArray uiVA;
	uiVA.AddBuffer(uiVB, uiLayout);

	const char* glsl_version = "#version 460";

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);

	float center_x = window_width / 2.0f;
	float center_y = window_height / 2.0f;

	ImGui::SetNextWindowPos(ImVec2(center_x, center_y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	//Time
	float deltaTime = 0.0f;
	float currentFrame = 0.0f;
	float lastFrame = 0.0f;

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	//Block* chunk = generateChunk();
	//printChunk(chunk);
	
	//sChunk.fillSuperchunk();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/*Render here */
		//glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cam->SetCameraSpeed(deltaTime);

		pc->ProcessInputs();

		//glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 projection = cam->ReturnProjection();
		glm::mat4 view = cam->ReturnView();
		glm::mat4 model = cam->ReturnModel();

		//atlasShader.Unbind();

		glDepthMask(GL_FALSE);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Bind();
		//glUseProgram(skyboxShader);
		//glm::mat4 sbProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
		glm::mat4 sbView = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
		skyboxShader.SetUniformMatrix4fv("projection", 1, GL_FALSE, &projection[0][0]);
		skyboxShader.SetUniformMatrix4fv("view", 1, GL_FALSE, &sbView[0][0]);
		// skybox cube
		skyboxVA.Bind();
		skyboxTexture.Bind(GL_TEXTURE_CUBE_MAP, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		skyboxVA.Unbind();
		glDepthMask(GL_TRUE);; // set depth function back to default
		skyboxShader.Unbind();

		/*atlasShader.SetUniformMatrix4fv("projection", 1, GL_FALSE, &projection[0][0]);
		atlasShader.SetUniformMatrix4fv("view", 1, GL_FALSE, &view[0][0]);
		atlasShader.SetUniformMatrix4fv("model", 1, GL_FALSE, &model[0][0]);*/

		glm::mat4 pvm = projection * view * model;
		//atlasShader.SetUniformMatrix4fv("pvm", 1, GL_FALSE, &pvm[0][0]);

		//startingChunk.render();
		atlasShader.Bind();
		sChunk->render(cam, &atlasShader);
		atlasShader.Unbind();

		glDisable(GL_DEPTH_TEST);
		uiShader.Bind();
		texture.Bind(GL_TEXTURE_2D, 0);
		uiShader.SetUniform1i("u_Texture", 0);

		GLuint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currentProgram);
		//std::cout << "Current Shader ID: " << currentProgram << std::endl;

		uiVA.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		uiVA.Unbind(); // set depth function back to default
		uiShader.Unbind();
		glEnable(GL_DEPTH_TEST);

		if(pc->inventoryToggle == true)
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Blocks");                          // Create a window called "Hello, world!" and append into it.

			renderBlockSelection(pc);
			
			ImGui::End();
		}

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	//delete pc;
	//delete sChunk;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}