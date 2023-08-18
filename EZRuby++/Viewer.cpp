#include "Viewer.h"
#include <iostream>
#include <map>

using namespace EzRuby;

ViewerV1::ViewerV1(const Cube& cube) : _cube(cube)
{
}

ViewerV1::~ViewerV1()
{
	graphicsDeleteSquare();
}

int ViewerV1::showWindow()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	// glfw window creation
	// --------------------
	_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowPos(_window, 200, 200);
	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// initializations
	graphicsInitSquare();

	// render loop
	while (!glfwWindowShouldClose(_window))
	{
		processInput(_window);
		render();
		glfwSwapBuffers(_window); // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();
	}

	// ------------------------------------------------------------------
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ViewerV1::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void ViewerV1::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void ViewerV1::graphicsInitSquare()
{
	if (_initialized)
		return;
	else
		_initialized = true;

	const size_t verticesSize = 12,
		indicesSize = 6;
	
	_vertices = new float[verticesSize] {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};

	_indices = new unsigned int[indicesSize] {
		0, 1, 2,
		2, 3, 0
	};

	// Compilation et liaison des shaders
	const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
		uniform mat4 transform;

        void main() {
            gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
	const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
		uniform vec3 color;

        void main() {
            FragColor = vec4(color, 1.0f);
        }
    )";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glLinkProgram(_shaderProgram);
	glUseProgram(_shaderProgram);

	// VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// VBO
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), _vertices, GL_STATIC_DRAW);

	// EBO
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), _indices, GL_STATIC_DRAW);

	// Config vertex positions attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void ViewerV1::graphicsDeleteSquare() {
	_initialized = false;
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteProgram(_shaderProgram);
	delete[] _vertices;
	delete[] _indices;
}

void ViewerV1::render() {
	glClearColor(0.0f, 0.05f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	drawCube();
}

void ViewerV1::drawSquare(glm::vec2 position, float size, glm::vec3 color) {
	glBindVertexArray(_vao);

	// create transformations
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// proj matrix
	int windowWidth, windowHeight;
	glfwGetFramebufferSize(_window, &windowWidth, &windowHeight);
	float aspectRatio = static_cast<float>(windowWidth) / windowHeight;
	glm::mat4 projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);
	transform *= projectionMatrix;
	// square position/size
	transform = glm::translate(transform, glm::vec3(position, 0.0f));
	transform = glm::scale(transform, glm::vec3(size));
	// get matrix's uniform location and set matrix
	glUseProgram(_shaderProgram);
	unsigned int transformLoc = glGetUniformLocation(_shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// set color
	unsigned int colorLoc = glGetUniformLocation(_shaderProgram, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ViewerV1::drawCube() {
	// Making square position map
	std::map<int, glm::vec2> sqPosMap = {
		// green face
		{27, glm::vec2(1 * SQ_SIZE, 0)}, 
		{24, glm::vec2(1 * SQ_SIZE, 2 * SQ_SIZE)},
		{29, glm::vec2(1 * SQ_SIZE, -2 * SQ_SIZE)},
		{25, glm::vec2(3 * SQ_SIZE, 2 * SQ_SIZE)},
		{26, glm::vec2(5 * SQ_SIZE, 2 * SQ_SIZE)},
		{30, glm::vec2(3 * SQ_SIZE, -2 * SQ_SIZE)},
		{31, glm::vec2(5 * SQ_SIZE, -2 * SQ_SIZE)},
		{28, glm::vec2(5 * SQ_SIZE, 0 * SQ_SIZE)},
	};
	std::pair<int, glm::vec2> offsets[] = {
		{-8, glm::vec2(-6 * SQ_SIZE, 0)},
		{-16, glm::vec2(-12 * SQ_SIZE, 0)},
		{8, glm::vec2(6 * SQ_SIZE, 0)},
		{-24, glm::vec2(-6 * SQ_SIZE, 6 * SQ_SIZE)},
		{16, glm::vec2(-6 * SQ_SIZE, -6 * SQ_SIZE)},
	};
	for (int i = 0; i < Cube::FACE_COUNT - 1; i++) {
		// now, duplicate the green face but for other faces
		for (size_t j = 24; j <= 31; j++) {
			int indexOffset = offsets[i].first;
			int newIndex = j + indexOffset;
			glm::vec2 posOffset = offsets[i].second;
			glm::vec2 newPos = sqPosMap[j] + posOffset;
			sqPosMap.insert(std::make_pair(newIndex, newPos));
		}
	}

	Color faceColorOrder[] = { Color::Red, Color::Blue, Color::White, Color::Green, Color::Yellow, Color::Orange };

	std::map<Color, glm::vec3> glmColors = {
		{Color::Red, glm::vec3(1.0f, 0.0f, 0.0f)},
		{Color::Blue, glm::vec3(0.0f, 0.0f, 1.0f)},
		{Color::White, glm::vec3(1.0f, 1.0f, 1.0f)},
		{Color::Green, glm::vec3(0.0f, 1.0f, 0.0f)},
		{Color::Yellow, glm::vec3(1.0f, 1.0f, 0.0f)},
		{Color::Orange, glm::vec3(1.0f, 123.0f/255.0f, 25.0f/255.0f)},
	};

	for (size_t i = 0; i < Cube::SQ_COUNT; i++) {
		glm::vec2 position = sqPosMap[i];
		Color sqColor = _cube.getColorAt(i);
		drawSquare(position, SQ_SIZE, glmColors[sqColor]);

		// Middle square, to the right of the left square
		if (i % Cube::FACE_SQ_COUNT == 3) {
			Color currentDrawingColor = faceColorOrder[i / Cube::FACE_SQ_COUNT];
			glm::vec2 offset = glm::vec2(2 * SQ_SIZE, 0.0f);
			drawSquare(position + offset, SQ_SIZE, glmColors[currentDrawingColor]);
		}
	}
}