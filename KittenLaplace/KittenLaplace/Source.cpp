// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
// 引入GLFW库
#include <cstdlib>
#include <vector>
#include <iostream>  
#include <glm/vec3.hpp>// glm::vec3
#include <glm/geometric.hpp>// glm::cross, glm::normalize

// 引入SOIL库
#include <SOIL.h>
// 引入GLM库
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// 包含着色器加载库
#include "shader.h"
// 包含相机控制辅助类
#include "camera.h"
// 包含简单的obj读取文件
#include "simpleObjLoader.h"
//// 包含纹理加载类
//#include "texture.h"
// 定义程序常量
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 1000;
// 用于相机交互参数
GLfloat lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;

bool firstMouseMove = true;
bool keyPressedStatus[1024]; // 按键情况记录
GLfloat deltaTime = 0.0f; // 当前帧和上一帧的时间差
GLfloat lastFrame = 0.0f; 
MyObjLoader objloader;
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));

// 键盘回调函数原型声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// 鼠标移动回调函数原型声明
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚轮回调函数原型声明
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 左右按键
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// 键盘回调函数原型声明
void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode);

// 场景中移动
//void do_movement();

bool Rightclick = false;

int main(int argc, char** argv)
{

	if (!glfwInit())	// 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// 开启OpenGL 3.3 core profile
	std::cout << "Start OpenGL core profile version 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"Demo of loading obj model", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);

	// 注册窗口键盘事件回调函数
	//glfwSetKeyCallback(window, key_callback)
	glfwSetKeyCallback(window, keyFun);
	// 注册鼠标事件回调函数
	glfwSetCursorPosCallback(window, mouse_move_callback);
	// 注册鼠标滚轮事件回调函数
	glfwSetScrollCallback(window, mouse_scroll_callback);
	// 注册按下事件回调函数
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// 鼠标拖动事件
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	// 鼠标捕获 停留在程序内
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 初始化GLEW 获取OpenGL函数
	glewExperimental = GL_TRUE; // 让glew获取所有拓展函数
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置视口参数
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//视口变换将位于-1.0到1.0范围的坐标变换到由glViewport函数所定义的坐标范围内

	//Section1 从obj文件加载数据
	vector<Vertex> vertData;
	if (!objloader.loadFromFile("kitten_noisy.obj", vertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	// Section2 准备纹理
	//GLint textureId = TextureHelper::loadDDS("../../resources/models/cube/cube.dds");

	//// Section3 建立Mesh对象
	////Mesh mesh(vertData, textureId);
	//Mesh mesh(vertData);
	//// Section4 准备着色器程序
	//Shader shader("cube.vertex", "cube.frag");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 开始主循环
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents(); // 处理例如鼠标 键盘等事件
		//do_movement(); // 根据用户操作情况 更新相机属性

		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (Rightclick)
		{
			vertData = objloader.laplace();
			Rightclick = false;
		}

		Mesh mesh(vertData);
		Shader shader("cube.vertex", "cube.frag");
		shader.use();
		//glm::mat4 projection = glm::perspective(camera.mouse_zoom, //→缩放
		//	(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 projection = glm::perspective(45.0f, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 view = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model));

		//光源位置
		GLint lightPosLoc = glGetUniformLocation(shader.programId, "lightPos");
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightPosLoc, 1, 1, 1);

		// 这里填写场景绘制代码
		mesh.draw(shader); // 绘制物体

		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(window); // 交换缓存
	}
	// 释放资源
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keyPressedStatus[key] = true;
		else if (action == GLFW_RELEASE)
			keyPressedStatus[key] = false;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	//camera.handleMouseMove(xpos, ypos);
}

// 由相机辅助类处理鼠标滚轮控制
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.handleMouseScroll(yoffset);
}

void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode)
{

	if (nAction == GLFW_PRESS)
	{
		//平移
		if (nKey == GLFW_KEY_Q)
		{
			camera.handleTranslation(BACKWARD);
		}
		else if (nKey == GLFW_KEY_E)
		{
			camera.handleTranslation(FORWARD);
		}
		else if (nKey == GLFW_KEY_A)
		{
			camera.handleTranslation(LEFT);
		}
		else if (nKey == GLFW_KEY_D)
		{
			camera.handleTranslation(RIGHT);
		}
		else if (nKey == GLFW_KEY_W)
		{
			camera.handleTranslation(UP);
		}
		else if (nKey == GLFW_KEY_S)
		{
			camera.handleTranslation(DOWN);
		}
		//旋转
		else if (nKey == GLFW_KEY_Z) //y轴顺时针
		{
			camera.handleTranslation(yClockwise);
		}
		else if (nKey == GLFW_KEY_C)//y轴逆时针
		{
			camera.handleTranslation(yAnticlockwise);
		}
		else if (nKey == GLFW_KEY_X)    // 还原视图
		{
			camera.handleTranslation(REBOUND);
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	GLfloat xpos = float((x - WINDOW_WIDTH / 2) / WINDOW_WIDTH) * 2;
	GLfloat ypos = float(0 - (y - WINDOW_HEIGHT / 2) / WINDOW_HEIGHT) * 2;
	//camera.handleTranslation(xpos, ypos);
	return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		//旋转
		break;
		/*case GLFW_MOUSE_BUTTON_MIDDLE:
		strcpy(msg, "Mosue middle button clicked!");
		break;*/
	case GLFW_MOUSE_BUTTON_RIGHT:
		//拉普拉斯
		Rightclick = true;
		break;
	default:
		return;
	}
	return;
}
