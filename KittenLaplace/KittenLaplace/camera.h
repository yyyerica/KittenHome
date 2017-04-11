#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <fstream>
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>      // std::setprecision

//�����ƶ�����
enum Camera_Movement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	yClockwise,
	yAnticlockwise,
	REBOUND
};

const GLfloat MOUSE_ZOOM = 45.0f; //����perspective����

class Camera
{
public:
	//Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0),
	//	glm::vec3 viewup = glm::vec3(0.0, 1.0, 0.0),
	//	glm::vec3 targetpos = glm::vec3(0.0, 0.0, 0.0))
	//	:Position(pos), TargetPos(targetpos), Viewup(viewup), mouse_zoom(MOUSE_ZOOM)
	//{

	//}

	Camera()
		:Position(glm::vec3(0.0, 0.0, 2.0)), TargetPos(glm::vec3(0.0, 0.0, 0.0)), 
		Viewup(glm::vec3(0.0, 1.0, 0.0)), mouse_zoom(MOUSE_ZOOM)
	{

	}


public:
	
	// ��ȡ�ӱ任����
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(this->Position, this->TargetPos, this->Viewup);
	}

	void handleMouseScroll(GLfloat yoffset)
	{
		if (this->mouse_zoom >= 1.0f && this->mouse_zoom <= MOUSE_ZOOM)
			this->mouse_zoom -= 0.05 * yoffset;
		if (this->mouse_zoom <= 1.0f)
			this->mouse_zoom = 1.0f;
		if (this->mouse_zoom >= 45.0f)
			this->mouse_zoom = 45.0f;
	}

	void handleMouseTranslation(GLdouble x, GLdouble y)
	{
		Position = glm::vec3(-x, -y, 2);
		TargetPos = glm::vec3(-x, -y, 0);
	}

	void handleRotation(GLdouble x, GLdouble y)
	{
		Position = glm::vec3(glm::sin(x * 180) , glm::sin(y * 90), glm::cos(x * 180));
		//Viewup = ;
		//std::cout << "x : " << Position.x << "y : " << Position.y << std::endl;
	}

	//�������
public:
	glm::vec3 Position, Viewup, TargetPos;
	GLfloat fRotateAngle = 0.0f;
	GLfloat mouse_zoom;
	//GLfloat xAngle, yAngle;
};

//// �����ƶ�����
//enum Camera_Movement {
//	FORWARD,
//	BACKWARD,
//	LEFT,
//	RIGHT
//};
//// ����Ԥ�賣��
//const GLfloat YAW = 0.0f;
//const GLfloat PITCH = 0.0f;
//const GLfloat SPEED = 3.0f;
//const GLfloat MOUSE_SENSITIVTY = 0.05f;
//const GLfloat MOUSE_ZOOM = 45.0f;
//const float  MAX_PITCH_ANGLE = 89.0f; // ��ֹ������
//
//class Camera
//{
//public:
//	Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0),
//		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0),
//		GLfloat yaw = YAW, GLfloat pitch = PITCH)
//		:position(pos), forward(0.0, 0.0, -1.0), viewUp(up),
//		moveSpeed(SPEED), mouse_zoom(MOUSE_ZOOM), mouse_sensitivity(MOUSE_SENSITIVTY),
//		yawAngle(yaw), pitchAngle(pitch)
//	{
//		this->updateCameraVectors();
//	}
//
//public:
//	// ��ȡ�ӱ任����
//	glm::mat4 getViewMatrix()
//	{
//		return glm::lookAt(this->position, this->position + this->forward, this->viewUp);
//	}
//	
//	// ������̰��������ƶ�
//	void handleKeyPress(Camera_Movement direction, GLfloat deltaTime)
//	{
//	//	GLfloat velocity = this->moveSpeed * deltaTime;
//	//	switch (direction)
//	//	{
//	//	case FORWARD:
//	//		this->position += this->forward * velocity;
//	//		break;
//	//	case BACKWARD:
//	//		this->position -= this->forward * velocity;
//	//		break;
//	//	case LEFT:
//	//		this->position -= this->side * velocity;
//	//		break;
//	//	case RIGHT:
//	//		this->position += this->side * velocity;
//	//		break;
//	//	default:
//	//		break;
//	//	}
//	//}
//	
//	//// ��������ƶ�
//	//void handleMouseMove(GLfloat xoffset, GLfloat yoffset)
//	//{
//	//	xoffset *= this->mouse_sensitivity; // ����������ȵ��ڽǶȱ任
//	//	yoffset *= this->mouse_sensitivity;
//
//	//	this->pitchAngle += yoffset;
//	//	this->yawAngle += xoffset;
//
//	//	this->normalizeAngle();
//	//	this->updateCameraVectors();
//	//}
//	void handleMouseMove(GLfloat x, GLfloat y)
//	{
//		glm::vec3 forward;
//		forward.x = x;
//		forward.y = y;
//		forward.z = 0;
//		this->forward = glm::normalize(forward);
//	}
//	
//	// �������������� ������[1.0, MOUSE_ZOOM]֮��
//	void handleMouseScroll(GLfloat yoffset)
//	{
//		/*if (this->mouse_zoom >= 1.0f && this->mouse_zoom <= MOUSE_ZOOM)
//			this->mouse_zoom -= this->mouse_sensitivity * yoffset;
//		if (this->mouse_zoom <= 1.0f)
//			this->mouse_zoom = 1.0f;
//		if (this->mouse_zoom >= 45.0f)
//			this->mouse_zoom = 45.0f;*/
//	}
//
//	// ʹpitch yaw�Ƕȱ����ں���Χ��
//	void normalizeAngle()
//	{
//		/*if (this->pitchAngle > MAX_PITCH_ANGLE)
//			this->pitchAngle = MAX_PITCH_ANGLE;
//		if (this->pitchAngle < -MAX_PITCH_ANGLE)
//			this->pitchAngle = -MAX_PITCH_ANGLE;
//		if (this->yawAngle < 0.0f)
//			this->yawAngle += 360.0f;*/
//	}
//	
//	// ����forward side����
//	void updateCameraVectors()
//	{
//		/*glm::vec3 forward;
//		forward.x = -sin(glm::radians(this->yawAngle)) * cos(glm::radians(this->pitchAngle));
//		forward.y = sin(glm::radians(this->pitchAngle));
//		forward.z = -cos(glm::radians(this->yawAngle)) * cos(glm::radians(this->pitchAngle));
//		this->forward = glm::normalize(forward);
//
//		glm::vec3 side;
//		side.x = cos(glm::radians(this->yawAngle));
//		side.y = 0;
//		side.z = -sin(glm::radians(this->yawAngle));
//		this->side = glm::normalize(side);*/
//	}
//
//public:
//	glm::vec3 forward, up, side, viewUp, position; // �������
//	GLfloat yawAngle, pitchAngle; // ŷ����
//	GLfloat moveSpeed, mouse_sensitivity, mouse_zoom; // ���ѡ��
//};

#endif