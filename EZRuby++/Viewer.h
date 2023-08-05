#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Viewer
{
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow *_window;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	void drawCube();

public:
	Viewer() = default;
	int ShowWindow();
};

