#include "Viewer.h"
#include <iostream>
#include <map>

using namespace EzRuby;

Viewer::Viewer(const Cube& cube) : _cube(cube)
{
}

Viewer::~Viewer()
{
	graphicsDeleteSquare();
}

int Viewer::showWindow()
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


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Viewer::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Viewer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Viewer::graphicsInitSquare()
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

void Viewer::graphicsDeleteSquare() {
	_initialized = false;
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteProgram(_shaderProgram);
	delete[] _vertices;
	delete[] _indices;
}

void Viewer::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	drawCube();
}

void Viewer::drawSquare(glm::vec2 position, float size, glm::vec3 color) {
	// create transformations
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(position, 0.0f));
	transform = glm::scale(transform, glm::vec3(size));

	// get matrix's uniform location and set matrix
	glUseProgram(_shaderProgram);
	unsigned int transformLoc = glGetUniformLocation(_shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// set color
	unsigned int colorLoc = glGetUniformLocation(_shaderProgram, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	// render container
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Viewer::drawCube() {
	// TODO: draw the full cube now
	/*glm::vec3 color1 = glm::vec3(0, 0, 1);
	drawSquare(-0.9f, -0.9f, 0.1f, color1);
	glm::vec3 color2 = glm::vec3(1, 0, 0);
	drawSquare(0.0f, -0.9f, 0.1f, color2);
	glm::vec3 color3 = glm::vec3(0, 1, 0);
	drawSquare(0.9f, -0.9f, 0.1f, color3);*/

	const float sqSize = 0.075f; // it represents only half
	// green 27 at 0.0f,0.0f
	// green side
	glm::vec3 greenColor = glm::vec3(0.0f, 1.0f, 0.0f);
	std::map<int, glm::vec2> sqMap = {
		{27, glm::vec2(sqSize, 0.0f)}, {24, glm::vec2(sqSize, 2 * sqSize)}
	};

	/*Cube::FACE_SQ_COUNT*/
	//std::map<Color, std::vec3>



	for (size_t i = 0; i < Cube::SQ_COUNT; i++) {
		bool isDefined = (sqMap.find(i) != sqMap.end()); // temp
		if (!isDefined) continue;

		glm::vec2 position = sqMap[i];
		drawSquare(position, sqSize, greenColor);
	}
}