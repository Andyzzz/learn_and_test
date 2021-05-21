// Pro2_draw_triangles.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// ���ڿ�&��
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ������ɫ����Ƭ����ɫ����д��const char���͵��ַ�����
// gl_Position�ǹ̶���vec4���ͣ��Ƕ�����ɫ�������
// in��out��ʾ��������
// out������FragColor����
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
	// glfw��ʼ��
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw ���ڴ���
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: �������е�opengl����ָ��
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// �����ͱ�����ɫ������
	// ------------------------------------
	// ������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //����һ��������ɫ������IDΪvertexShader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   //�Ѷ�����ɫ�������ݸ���������ɫ������
	glCompileShader(vertexShader);   // ������ɫ��
	// �������Ƿ�ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Ƭ����ɫ��������������
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ������ɫ��
	unsigned int shaderProgram = glCreateProgram();  // ͬ����Ҳ���ȴ���һ������
	glAttachShader(shaderProgram, vertexShader);    // �Ѷ����Ƭ����ɫ����ID�󶨵����������
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);  // ����
	// ��������Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ���嶥�����ݣ����ö�������
	// ע�⣺�������ֵ��Χ��һ���� -1.0~1.0
	// 2Dƽ��Ķ�����԰�z��Ϊ0
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;      //���嶥�㻺����󣬶����������
	glGenVertexArrays(1, &VAO); //���ɶ����������
	glGenBuffers(1, &VBO);      //���ɶ��㻺�������GPU�д洢�������ݵĵط���
	// �Ȱ󶨶����������Ȼ������ö��㻺�����ͣ�������ö�������
	glBindVertexArray(VAO);
	// ���㻺���������ͣ���ΪGL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �Ѷ���Ķ�������vertices���Ƶ���ǰ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ָ���������ݸ�ʽ��float���ͣ�3��Ϊһ�飬ռ���ֽ���Ϊ3*sizeof(float)
	// Ϊopengl���������������vertices����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��һ������ָ��Ҫ���õĶ������ԡ�
	//�ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)
	//���԰Ѷ������Ե�λ��ֵ����Ϊ0����ʾ�����ݴ��ݵ���һ������������

	// ���㻺����
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// �������������
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// ��������
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ����
		// ʹ����ɫ������ ֮ǰ�Ѿ����롢���ӹ�
		glUseProgram(shaderProgram);
		// ��VAO�����������
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		// ����ͼ�Σ�����glDrawElements��ģʽ��������GL_TRIANGLES,
		glDrawArrays(GL_TRIANGLES, 0, 3);  // �����3��ָ���������Ϊ3
		//GL_TRIANGLES��ÿ������֮����������Σ�֮�䲻����
		//GL_TRIANGLE_FAN����V0V1V2,V0V2V3,V0V3V4����������ʽ����������
		//GL_TRIANGLE_STRIP��˳����ÿ��������֮������������Ρ���V0V1V2,V1V2V3,V2V3V4��������ʽ����������
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

