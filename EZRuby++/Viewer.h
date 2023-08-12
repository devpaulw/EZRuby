#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"

namespace EzRuby {
	class Viewer
	{
	private:
		// settings
		const unsigned int SCR_WIDTH = 800;
		const unsigned int SCR_HEIGHT = 800;//600; temp

		const Cube& _cube;
		bool _initialized;
		GLFWwindow* _window;
		unsigned int _vao, _vbo, _ebo;
		float* _vertices;
		unsigned int* _indices;
		unsigned int _shaderProgram;

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void processInput(GLFWwindow* window);
		void graphicsInitSquare();
		void graphicsDeleteSquare();
		void drawSquare(glm::vec2 position, float size, glm::vec3 color);
		void drawCube();
		void render();

	public:
		Viewer(const Cube& cube);
		~Viewer();
		int showWindow();
	};
}