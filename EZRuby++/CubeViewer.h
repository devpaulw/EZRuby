#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CubeViewer
{
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);

public:
	CubeViewer() = default;
	int ShowWindow();
};

