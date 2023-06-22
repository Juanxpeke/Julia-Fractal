#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "helper_math.h"
#include "file_manager.h"

int width = 720;
int height = 720;
char title[256];

std::string shaderNames[] = { "quadratric", "cubic", "exponential" };
GLuint shaders[3];
int currentShaderIdx = 0;

float2 c = { -0.54f, 0.54f };

int bailout = 128;
float limit = 50.0f;

float color = 0.0f;
float zoom = 1.0f;
float2 translation = { 0.0f, 0.0f };

void printConfiguration()
{
  std::cout <<"Configuration" << std::endl;
  std::cout <<"=============" << std::endl;
	std::cout << "shader: " << shaderNames[currentShaderIdx] << std::endl;
  std::cout << "c: (" << c.x << ", " << c.y << ")" << std::endl;
  std::cout << "bailout: " << bailout << std::endl;
  std::cout << "limit: " << limit << std::endl;
  std::cout << "zoom: " << zoom << std:: endl << std:: endl;
}

void updateInputs(GLFWwindow* window)
{
  bool configurationChanged = false;
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { currentShaderIdx = 0; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) { currentShaderIdx = 1; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) { currentShaderIdx = 2; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { c.x += 0.005f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { c.x -= 0.005f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { c.y += 0.005f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { c.y -= 0.005f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { limit += 2.0f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) { limit -= 2.0f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { zoom *= 1.2f; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) { zoom /= 1.2f; configurationChanged = true; }
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { color += 0.1f; configurationChanged = true; }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { translation.x += 0.01f / zoom; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { translation.x -= 0.01f / zoom; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { translation.y += 0.01f / zoom; configurationChanged = true; }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { translation.y -= 0.01f / zoom; configurationChanged = true; }
  if (configurationChanged) printConfiguration();
}

int main() {
  // Print initial configuration
  printConfiguration();

	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Vertices coordinates
	GLfloat vertices[] = {
    -1.0f,  1.0f, 0.0f, // Upper left corner
		-1.0f, -1.0f, 0.0f, // Lower left corner
		 1.0f, -1.0f, 0.0f, // Lower right corner
     1.0f,  1.0f, 0.0f, // Upper right corner
		-1.0f,  1.0f, 0.0f, // Upper left corner
     1.0f, -1.0f, 0.0f  // Lower right corner
	};

  sprintf(title, "Julia Fractals");
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	glViewport(0, 0, width, height);

	// Vertex shader
  std::string vertexShaderCode = getFileContent("../../shaders/julia_fractal.vert");
	const char* vertexShaderSource = vertexShaderCode.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Fragment shader
  std::string fragmentShaderCode = getFileContent("../../shaders/julia_fractal_2.frag");
	const char* fragmentShaderSource = fragmentShaderCode.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Fragment shader 2
  std::string fragmentShaderCode2 = getFileContent("../../shaders/julia_fractal_3.frag");
	const char* fragmentShaderSource2 = fragmentShaderCode2.c_str();
	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	// Fragment shader 3
  std::string fragmentShaderCode3 = getFileContent("../../shaders/julia_fractal_e.frag");
	const char* fragmentShaderSource3 = fragmentShaderCode3.c_str();
	GLuint fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
	glCompileShader(fragmentShader3);

	// Shader program 1
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	shaders[0] = shaderProgram;

	// Shader program 2
	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	shaders[1] = shaderProgram2;

	// Shader program 3
	GLuint shaderProgram3 = glCreateProgram();
	glAttachShader(shaderProgram3, vertexShader);
	glAttachShader(shaderProgram3, fragmentShader3);
	glLinkProgram(shaderProgram3);
	shaders[2] = shaderProgram3;

	// Delete the now useless Vertex and Fragment Shader Objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);

	// Create reference containers for the Vertex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(shaderProgram, "position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "position"));

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Specify the color of the background
	glClearColor(0.27f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assing the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

  // Bind the VAO so OpenGL knows to use it
	glBindVertexArray(VAO);

	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

    updateInputs(window);

		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaders[currentShaderIdx]);

    // Set uniforms
		glUniform2f(glGetUniformLocation(shaderProgram, "resolution"), width, height);
    glUniform2f(glGetUniformLocation(shaderProgram, "c"), c.x, c.y);
    glUniform1i(glGetUniformLocation(shaderProgram, "bailout"), bailout);
    glUniform1f(glGetUniformLocation(shaderProgram, "limit"), limit);
    glUniform1f(glGetUniformLocation(shaderProgram, "color"), color);
    glUniform1f(glGetUniformLocation(shaderProgram, "zoom"), zoom);
		glUniform2f(glGetUniformLocation(shaderProgram, "translation"), translation.x, translation.y);

		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
    
    glfwSwapBuffers(window);
	}

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}