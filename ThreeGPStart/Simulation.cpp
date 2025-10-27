#include "Simulation.h"
#include "Camera.h"
#include "Renderer.h"
#include <chrono>


// Initialise this as well as the renderer, returns false on error
bool Simulation::Initialise()
{
	//Set up camera
	m_camera = std::make_shared<Helpers::Camera>();
	m_camera->Initialise(glm::vec3(-2000, 2500, 4000), glm::vec3(.40, .45, 0)); // Cube

	std::cout << "Other Controls" << std::endl;
	std::cout << "F5 - reloads shaders" << std::endl << std::endl;

	// Set up renderer
	m_renderer = std::make_shared<Renderer>();
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
	}

	return m_renderer->InitialiseGeometry(0);
}
//restart simulation
bool Simulation::Restart()
{
	round += 1;
	std::cout << round << std::endl;
	// Set up renderer
	m_renderer = std::make_shared<Renderer>();
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
	}
	return m_renderer->InitialiseGeometry(round);
}


// Handle any user input. Return false if program should close.
bool Simulation::HandleInput(GLFWwindow* window)
{	
	// Not if it is being handled by IMGUI
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.WantCaptureKeyboard || io.WantCaptureMouse)
		return true;

	// F5 to reload shaders
	if ((glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS))
		m_renderer->CreateProgram();
	
	return true;
}

// Update the simulation (and render) returns false if program should close
bool Simulation::Update(GLFWwindow* window)
{
	// Deal with any input
	if (!HandleInput(window))
		return false;

	// Calculate delta time since last called
	// We pass the delta time to the camera and renderer
	float timeNow = (float)glfwGetTime();
	float deltaTime{ timeNow - m_lastTime };
	m_lastTime = timeNow;

	// The camera needs updating to handle user input internally
	m_camera->Update(window, deltaTime);

	m_renderer->Render(*m_camera, deltaTime);
	m_renderer->UpdatePhysics(deltaTime);

	// IMGUI	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	m_renderer->DefineGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}
