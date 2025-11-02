#include "Simulation.h"



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

	return m_renderer->InitialiseGeometry();
}

//restart simulation
bool Simulation::Restart()
{
	// Set up renderer
	m_renderer = std::make_shared<Renderer>();
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
	}

	m_renderer->m_isPaused = false;

	return m_renderer->InitialiseGeometry();
}

bool IsKeyPressedOnce(GLFWwindow* window, int key)
{
	static std::unordered_map<int, bool> previousState;

	bool isPressed = (glfwGetKey(window, key) == GLFW_PRESS);
	bool wasPressed = previousState[key];

	previousState[key] = isPressed;

	return (isPressed && !wasPressed);
}

// Handle any user input. Return false if program should close.
bool Simulation::HandleInput(GLFWwindow* window)
{	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.WantCaptureKeyboard || io.WantCaptureMouse)
		return true;

	// Reload shaders (F5)
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
		m_renderer->CreateProgram();

	// Restart (R)
	if (IsKeyPressedOnce(window, GLFW_KEY_R))
	{
		std::cout << "Restart\n";
		Restart();
	}

	// Pause toggle (P)
	if (IsKeyPressedOnce(window, GLFW_KEY_P))
	{
		std::cout << "Pause toggled\n";
		m_renderer->m_isPaused = !m_renderer->m_isPaused;
	}

	return true;
}

// Update the simulation (and render) returns false if program should close
bool Simulation::Update(GLFWwindow* window)
{		
	// Deal with any input
	if (!HandleInput(window))
		return false;

	if (m_renderer->m_shouldRestart)
		Restart();

	// Calculate delta time since last called
	// We pass the delta time to the camera and renderer
	float timeNow = (float)glfwGetTime();
	float deltaTime{ timeNow - m_lastTime };
	m_lastTime = timeNow;

	// The camera needs updating to handle user input internally
	m_camera->Update(window, deltaTime);
	m_renderer->Render(*m_camera, deltaTime);
	// IMGUI	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	m_renderer->DefineGUI();

	ImGui::Render();
	if (!m_renderer->m_isPaused)
		m_renderer->UpdatePhysics(deltaTime);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}
