#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"

namespace EzRuby {
	// ViewerV1 draws a lightweight 2D pattern of the cube
	class ViewerV1 
	{
	private:
		// settings
		const unsigned int SCR_WIDTH = (unsigned int)(800.0f*1.2f);
		const unsigned int SCR_HEIGHT = (unsigned int)(600.0f*1.2f);
		const float SQ_SIZE = 0.095f;

		const Cube& _cube;
		bool _initialized;
		GLFWwindow* _window;
		unsigned int _vao, _vbo, _ebo;
		float* _vertices;
		unsigned int* _indices;
		unsigned int _shaderProgram;
		glm::mat4 _projectionMatrix;

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void processInput(GLFWwindow* window);
		void graphicsInitSquare();
		void graphicsDeleteSquare();
		void drawSquare(glm::vec2 position, float size, glm::vec3 color);
		void drawCube();
		void render();

	public:
		ViewerV1() = delete;
		ViewerV1(const Cube& cube);
		~ViewerV1();
		int showWindow();
	};
}