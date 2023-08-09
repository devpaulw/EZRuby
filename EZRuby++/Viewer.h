#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Viewer
{
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	bool _initialized;
	GLFWwindow *_window;
	unsigned int _vao, _vbo, _ebo;
	float *_vertices;
	unsigned int *_indices;
	unsigned int _shaderProgram;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	void graphicsInitSquare();
	void graphicsDeleteSquare();
	void drawSquare(float posX, float posY, float size);
	void drawCube();
	void render();

public:
	Viewer();
	~Viewer();
	int showWindow();
};

