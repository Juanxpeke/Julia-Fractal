#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "helper_math.h"
#include "file_manager.h"

int width = 720;
int height = 720;

float2 c = { -0.75f, 0.20f };

int bailout = 4096;
float limit = 3.0f;

float zoom = 1.0f;

void updateInputs(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) c.x += 0.01f;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) c.x -= 0.01f;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) c.y += 0.01f;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) c.y -= 0.01f;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) limit += 2.0f;
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) limit -= 2.0f;
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) zoom *= 1.5f;
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) zoom /= 1.5f;
}

int main() {

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

	GLFWwindow* window = glfwCreateWindow(width, height, "Julia Fractals", NULL, NULL);

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

  std::string vertexShaderCode = getFileContent("../../shaders/julia_fractal.vert");
  std::string fragmentShaderCode = getFileContent("../../shaders/julia_fractal.frag");

  const char* vertexShaderSource = vertexShaderCode.c_str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Vertex Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Fragment Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up / Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader Objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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

	// Tell OpenGL which Shader Program we want to use
	glUseProgram(shaderProgram);
	// Bind the VAO so OpenGL knows to use it
	glBindVertexArray(VAO);

  // Set uniforms
  glUniform2f(glGetUniformLocation(shaderProgram, "resolution"), width, height);

	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

    updateInputs(window);

		glClear(GL_COLOR_BUFFER_BIT);

    // Set uniforms
    glUniform2f(glGetUniformLocation(shaderProgram, "c"), c.x, c.y);
    glUniform1i(glGetUniformLocation(shaderProgram, "bailout"), bailout);
    glUniform1f(glGetUniformLocation(shaderProgram, "limit"), limit);
    glUniform1f(glGetUniformLocation(shaderProgram, "zoom"), zoom);


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