
#include <cstdint>
#include <iostream>

#include "World.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <sys/stat.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

off_t GetFileSize(int fd)
{
	struct stat buf;
	int ret = fstat(fd, &buf);
	return ret == 0 ? buf.st_size : -1;
}

GLuint LoadShaders(const char *vertex_file, const char *fragment_file)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	FILE *VertexShaderFile = fopen(vertex_file, "rb");
	FILE *FragmentShaderFile = fopen(fragment_file, "rb");

	if (!VertexShaderFile || !FragmentShaderFile)
	{
		if (VertexShaderFile) fclose(VertexShaderFile);
		if (FragmentShaderFile) fclose(FragmentShaderFile);
		return 0;
	}

	off_t vertex_size = GetFileSize(fileno(VertexShaderFile));
	off_t fragment_size = GetFileSize(fileno(FragmentShaderFile));

	char *VertexShaderCode = (char*)malloc(vertex_size + 1);
	char *FragmentShaderCode = (char*)malloc(fragment_size + 1);

	VertexShaderCode[vertex_size] = '\0';
	FragmentShaderCode[fragment_size] = '\0';
	
	fread(VertexShaderCode, 1, vertex_size, VertexShaderFile);
	fread(FragmentShaderCode, 1, fragment_size, FragmentShaderFile);

	fclose(VertexShaderFile);
	fclose(FragmentShaderFile);

	glShaderSource(VertexShaderID, 1, (const char**)&VertexShaderCode, nullptr);
	glCompileShader(VertexShaderID);

	glShaderSource(FragmentShaderID, 1, (const char**)&FragmentShaderCode, nullptr);
	glCompileShader(FragmentShaderID);

	GLint res = GL_FALSE;

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &res);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &res);

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &res);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	free(FragmentShaderCode);
	free(VertexShaderCode);

	return ProgramID;
}

int main(int argc, const char *argv[])
{
    std::cout << "Hello World!" << std::endl;

	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RED_BITS, 0);
	glfwWindowHint(GLFW_GREEN_BITS, 0);
	glfwWindowHint(GLFW_BLUE_BITS, 0);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);

	GLFWwindow* window = glfwCreateWindow(640, 480, "JORDAN", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint ProgramID = LoadShaders("../src/shaders/simplevertex.vs", "../src/shaders/simplefragment.fs");

	GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");
	
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 Model = glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f))
		* glm::rotate(30.0f, glm::vec3(0, 0, 1.0f))
		* glm::scale(glm::vec3(1.0f));
	glm::mat4 MVP = Projection * View * Model;

	static const GLfloat vertex_data[] = 
	{
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	static const GLfloat colour_data[] =
	{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	GLuint colourbuffer;
	glGenBuffers(1, &colourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colour_data), colour_data, GL_STATIC_DRAW);

    World w;
    w.Initialize();

	const uint32_t SERVER_FRAME_DT = 10000;

	while (!glfwWindowShouldClose(window))
	{
		w.Update(SERVER_FRAME_DT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ProgramID);
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
			glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_data) / 3 / sizeof(vertex_data[0]));

			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    w.Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colourbuffer);
	glDeleteProgram(ProgramID);
	glDeleteVertexArrays(1, &VertexArrayID);
}
