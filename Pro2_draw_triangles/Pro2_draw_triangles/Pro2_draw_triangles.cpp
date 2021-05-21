// Pro2_draw_triangles.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 窗口宽&高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器和片段着色器，写在const char类型的字符串里
// gl_Position是固定的vec4类型，是顶点着色器的输出
// in和out表示输入和输出
// out定义了FragColor变量
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
	// glfw初始化
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw 窗口创建
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

	// glad: 加载所有的opengl函数指针
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// 创建和编译着色器程序
	// ------------------------------------
	// 顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //创建一个顶点着色器对象，ID为vertexShader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   //把顶点着色器的内容赋给顶点着色器对象
	glCompileShader(vertexShader);   // 编译着色器
	// 检查编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 片段着色器，和上面类似
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
	// 链接着色器
	unsigned int shaderProgram = glCreateProgram();  // 同样，也是先创建一个对象
	glAttachShader(shaderProgram, vertexShader);    // 把顶点和片段着色器的ID绑定到这个对象上
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);  // 链接
	// 检查链接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 定义顶点数据，设置顶点属性
	// 注意：顶点的数值范围归一化到 -1.0~1.0
	// 2D平面的顶点可以把z设为0
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;      //定义顶点缓冲对象，顶点数组对象
	glGenVertexArrays(1, &VAO); //生成顶点数组对象
	glGenBuffers(1, &VBO);      //生成顶点缓冲对象【在GPU中存储顶点数据的地方】
	// 先绑定顶点数组对象，然后绑定设置顶点缓冲类型，最后设置顶点属性
	glBindVertexArray(VAO);
	// 顶点缓冲对象的类型，绑定为GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 把定义的顶点数据vertices复制到当前缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 指定顶点数据格式，float类型，3个为一组，占的字节数为3*sizeof(float)
	// 为opengl解释清楚怎样解析vertices数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//第一个参数指定要配置的顶点属性。
	//在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)
	//可以把顶点属性的位置值设置为0，表示把数据传递到这一个顶点属性中

	// 顶点缓冲解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 顶点数组对象解绑
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// 窗口输入
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绘制
		// 使用着色器程序 之前已经编译、链接过
		glUseProgram(shaderProgram);
		// 绑定VAO顶点数组对象
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		// 绘制图形，类似glDrawElements。模式是三角形GL_TRIANGLES,
		glDrawArrays(GL_TRIANGLES, 0, 3);  // 这里的3是指顶点的数量为3
		//GL_TRIANGLES：每三个顶之间绘制三角形，之间不连接
		//GL_TRIANGLE_FAN：以V0V1V2,V0V2V3,V0V3V4，……的形式绘制三角形
		//GL_TRIANGLE_STRIP：顺序在每三个顶点之间均绘制三角形。以V0V1V2,V1V2V3,V2V3V4……的形式绘制三角形
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

