// ����GLEW�� ���徲̬����
#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
// ����GLFW��
#include <cstdlib>
#include <vector>
#include <iostream>  
#include <glm/vec3.hpp>// glm::vec3
#include <glm/geometric.hpp>// glm::cross, glm::normalize

// ����SOIL��
#include <SOIL.h>
// ����GLM��
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// ������ɫ�����ؿ�
#include "shader.h"
// ����������Ƹ�����
#include "camera.h"
// �����򵥵�obj��ȡ�ļ�
#include "simpleObjLoader.h"
//// �������������
//#include "texture.h"
// ���������
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 1000;
// ���������������
GLfloat lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;

bool firstMouseMove = true;
bool keyPressedStatus[1024]; // ���������¼
GLfloat deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
GLfloat lastFrame = 0.0f; 
MyObjLoader objloader;
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));

// ���̻ص�����ԭ������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// ����ƶ��ص�����ԭ������
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
// �����ֻص�����ԭ������
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// ���Ұ���
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// ���̻ص�����ԭ������
void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode);

// �������ƶ�
//void do_movement();

bool Rightclick = false;

int main(int argc, char** argv)
{

	if (!glfwInit())	// ��ʼ��glfw��
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// ����OpenGL 3.3 core profile
	std::cout << "Start OpenGL core profile version 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"Demo of loading obj model", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// �����Ĵ��ڵ�contextָ��Ϊ��ǰcontext
	glfwMakeContextCurrent(window);

	// ע�ᴰ�ڼ����¼��ص�����
	//glfwSetKeyCallback(window, key_callback)
	glfwSetKeyCallback(window, keyFun);
	// ע������¼��ص�����
	glfwSetCursorPosCallback(window, mouse_move_callback);
	// ע���������¼��ص�����
	glfwSetScrollCallback(window, mouse_scroll_callback);
	// ע�ᰴ���¼��ص�����
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// ����϶��¼�
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	// ��겶�� ͣ���ڳ�����
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ��ʼ��GLEW ��ȡOpenGL����
	glewExperimental = GL_TRUE; // ��glew��ȡ������չ����
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// �����ӿڲ���
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//�ӿڱ任��λ��-1.0��1.0��Χ������任����glViewport��������������귶Χ��

	//Section1 ��obj�ļ���������
	vector<Vertex> vertData;
	if (!objloader.loadFromFile("kitten_noisy.obj", vertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	// Section2 ׼������
	//GLint textureId = TextureHelper::loadDDS("../../resources/models/cube/cube.dds");

	//// Section3 ����Mesh����
	////Mesh mesh(vertData, textureId);
	//Mesh mesh(vertData);
	//// Section4 ׼����ɫ������
	//Shader shader("cube.vertex", "cube.frag");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// ��ʼ��ѭ��
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents(); // ����������� ���̵��¼�
		//do_movement(); // �����û�������� �����������

		// �����ɫ������ ����Ϊָ����ɫ
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
		//glm::mat4 projection = glm::perspective(camera.mouse_zoom, //������
		//	(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // ͶӰ����
		glm::mat4 projection = glm::perspective(45.0f, //������
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // ͶӰ����
		glm::mat4 view = camera.getViewMatrix(); // �ӱ任���� ��lookAt  ��Ҫ�ƶ���ת������
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model));

		//��Դλ��
		GLint lightPosLoc = glGetUniformLocation(shader.programId, "lightPos");
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightPosLoc, 1, 1, 1);

		// ������д�������ƴ���
		mesh.draw(shader); // ��������

		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(window); // ��������
	}
	// �ͷ���Դ
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
		glfwSetWindowShouldClose(window, GL_TRUE); // �رմ���
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	//camera.handleMouseMove(xpos, ypos);
}

// ����������ദ�������ֿ���
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.handleMouseScroll(yoffset);
}

void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode)
{

	if (nAction == GLFW_PRESS)
	{
		//ƽ��
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
		//��ת
		else if (nKey == GLFW_KEY_Z) //y��˳ʱ��
		{
			camera.handleTranslation(yClockwise);
		}
		else if (nKey == GLFW_KEY_C)//y����ʱ��
		{
			camera.handleTranslation(yAnticlockwise);
		}
		else if (nKey == GLFW_KEY_X)    // ��ԭ��ͼ
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
		//��ת
		break;
		/*case GLFW_MOUSE_BUTTON_MIDDLE:
		strcpy(msg, "Mosue middle button clicked!");
		break;*/
	case GLFW_MOUSE_BUTTON_RIGHT:
		//������˹
		Rightclick = true;
		break;
	default:
		return;
	}
	return;
}
