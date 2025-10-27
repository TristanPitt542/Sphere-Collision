#include "ExternalLibraryHeaders.h"
#include "RedirectStandardOutput.h"
#include <thread>
#include <chrono>
#include "Helper.h"
#include "Simulation.h"

// Note: you should not need to edit any of this
int main()
{	
	// Allows cout to go to the output pane in Visual Studio rather than have to open a console window
	RedirectStandardOuput();

	// Use the provided helper function to set up GLFW, GLEW and OpenGL
	GLFWwindow* window{ Helpers::CreateGLFWWindow(1920, 1080, "Phyisics Simulation") };

	if (!window)
		return -1;

	// Create an instance of the simulation class and initialise it
	// If it could not load, exit gracefully
	Simulation simulation;	
	if (!simulation.Initialise())
	{
		glfwTerminate();
		return -1;
	}
		
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	auto startTime = std::chrono::steady_clock::now(); // Capture the start time

	// Enter main GLFW loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{		
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - startTime;


		if (!simulation.Update(window))
			break;

		// If 10 seconds have passed, reset the simulation
		if ((elapsedTime.count() >= 5.0f) && (simulation.GetRound() < 3))
		{
			simulation.Restart();
			startTime = std::chrono::steady_clock::now();
		}

		// GLFW updating
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Close down IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}