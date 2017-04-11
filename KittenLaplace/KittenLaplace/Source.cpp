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
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 800;
// ���������������
GLfloat lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;

int mode = 0;//0ƽ�ƣ�1��ת
glm::vec3 rotateVec = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouseMove = true;

MyObjLoader objloader;
Camera camera;

//��ת
GLfloat angley = 0.0f;
GLfloat anglex = 0.0f;

// ��갴���¼�
//void cursor_position_callback(GLFWwindow* window, double x, double y);
// ����ƶ��ص�����ԭ������
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
// �����ֻص�����ԭ������
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// ���Ұ���
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

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
		"Kitten_Noisy", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// �����Ĵ��ڵ�contextָ��Ϊ��ǰcontext
	glfwMakeContextCurrent(window);


	// ע������ƶ��¼��ص�����
	glfwSetCursorPosCallback(window, mouse_move_callback);
	// ע���������¼��ص�����
	glfwSetScrollCallback(window, mouse_scroll_callback);
	// ע�ᰴ���¼��ص�����
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// ����϶��¼�
	//glfwSetCursorPosCallback(window, cursor_position_callback);


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

	// �����ӿڲ��� (-1��1
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// ��ʼ��ѭ��
	while (!glfwWindowShouldClose(window))
	{
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

		glm::mat4 projection = glm::perspective(camera.mouse_zoom, //������
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // ͶӰ����
		glm::mat4 view = camera.getViewMatrix(); // �ӱ任���� ��lookAt  ��Ҫ�ƶ���ת������
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, angley, glm::vec3(0.0f, 1.0f,0.0f));
		model = glm::rotate(model, anglex, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model));

		//��Դλ��
		GLint lightPosLoc = glGetUniformLocation(shader.programId, "lightPos");
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightPosLoc, 0, 0, 5);

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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // �رմ���
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	double x = (2 * xpos) / WINDOW_WIDTH - 1;
	double y = 1 - (2 * ypos) / WINDOW_HEIGHT;
	if (mode == 0)
	{
		camera.handleMouseTranslation(x, y);
	}
	else if (mode == 1)
	{
		/*x *= 0.5;
		y *= 0.5;*/
		
		GLfloat xoffset = lastX - xpos;
		GLfloat yoffset = lastY - ypos;
		
		xoffset *= 0.05;
		yoffset *= 0.05;

		angley += 360 * (GLfloat)xoffset / (GLfloat)WINDOW_WIDTH;//������Ļ����껬���ľ�����������ת�ĽǶ�  
		anglex += 360 * (GLfloat)yoffset / (GLfloat)WINDOW_HEIGHT;//������Ļ����껬���ľ�����������ת�ĽǶ�  

		lastX = xpos;
		lastY = ypos;

	}
	
}

// ����������ദ�������ֿ���
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.handleMouseScroll(yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			//������˹
			Rightclick = true;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			//�л��ƶ���ת
			if (mode == 0)
				mode = 1;
			else if (mode == 1) {
				mode = 0;
			}
				
			break;
		default:
			return;
	}
	return;
}
