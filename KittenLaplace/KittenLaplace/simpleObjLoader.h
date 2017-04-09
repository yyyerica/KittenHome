#ifndef SIMPLE_OBJ_LOADER_H_
#define SIMPLE_OBJ_LOADER_H_
/*
* ��ֻ��һ��ʾ���ļ�obj������
* ��ʵ�õ�obj����Բ鿴https://github.com/syoyo/tinyobjloader
*/
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>       
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
//#include <stdlib.h>
//#include "shader.h"



using namespace glm;
using std::vector;

// ��ʾһ����������
struct Vertex
{
	vec3 position;
	vec3 normal;
};

struct SumNorStruct
{
	vec3 sumNormal;
	GLint sum = 0;
	//vector<vec3> Consecutive_points;//���ڵ�
	vector<GLint> Consecutive_points;
};

// ��ʾһ��������Ⱦ����Сʵ��
class Mesh
{
public:
	void draw(Shader& shader) // ����Mesh
	{
		shader.use();
		glBindVertexArray(this->VAOId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);

		glDrawArrays(GL_TRIANGLES, 0, this->vertData.size());

		glBindVertexArray(0);
		glUseProgram(0);
	}
	Mesh(){}

	Mesh(const std::vector<Vertex>& vertData, bool bShowData = false) // ����һ��Mesh
	{
		this->vertData = vertData;
		//this->textureId = textureId;
		this->setupMesh();
		if (bShowData)
		{
			const char * fileName = "vert-data.txt";
			std::ofstream file(fileName);
			for (std::vector<Vertex>::const_iterator it = this->vertData.begin(); it != this->vertData.end(); ++it)
			{
				file << glm::to_string(it->position) << ","
					//<< glm::to_string(it->texCoords) << ","
					<< glm::to_string(it->normal) << std::endl;
			}
			file.close();
			std::cout << " vert data saved in file:" << fileName << std::endl;
		}
	}
	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAOId);
		glDeleteBuffers(1, &this->VBOId);
	}
public:
	std::vector<Vertex> vertData;
	GLuint VAOId, VBOId;
	GLint textureId;

	void setupMesh()  // ����VAO,VBO�Ȼ�����
	{
		glGenVertexArrays(1, &this->VAOId);
		glGenBuffers(1, &this->VBOId);

		glBindVertexArray(this->VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* this->vertData.size(),
			&this->vertData[0], GL_STATIC_DRAW);
		// ����λ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class MyObjLoader
{
	
	vector<Vertex> primaryVector;//���յ��˳��ӵ�ж���������
	vector<int> vertice_index;
	vector<vec3> temp_vertices;

public:
	bool loadFromFile(const char* path,
		vector<Vertex>& vertData)
	{	
		int v1, v2, v3;
		int faceCount = 0;
		SumNorStruct Norsum[40000];

		vector<Vertex> vertData1;//������˳���Ĵ��з������Ķ����б�
		vec3 normal1;
		
		std::ifstream file(path);
		if (!file)
		{
			std::cerr << "Error::ObjLoader, could not open obj file:"
				<< path << " for reading." << std::endl;
			return false;
		}
		std::string line;
		while (getline(file, line))
		{
			if (line.substr(0, 1) == "v") // ����λ������
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				temp_vertices.push_back(v);
			}
			else if (line.substr(0, 1) == "f") // ������
			{
				std::istringstream vtns(line.substr(2));
				std::string vtn;
				while (vtns >> vtn) // ����һ���ж����������
				{
					int vertComIndex;
					std::replace(vtn.begin(), vtn.end(), '/', ' ');
					std::istringstream ivtn(vtn);

					ivtn >> vertComIndex;
					vertComIndex--;
					vertice_index.push_back(vertComIndex);
					
					switch (faceCount % 3)
					{
					case 0:
						v1 = vertComIndex;
						break;
					case 1:
						v2 = vertComIndex;
						break;
					case 2:
						v3 = vertComIndex;

						normal1 = cross(temp_vertices[v2] - temp_vertices[v1], temp_vertices[v3] - temp_vertices[v1]);

						Norsum[v1].sum++;
						Norsum[v1].sumNormal += normal1;
						if (find(Norsum[v1].Consecutive_points.begin(), Norsum[v1].Consecutive_points.end(), v2) == Norsum[v1].Consecutive_points.end())
							Norsum[v1].Consecutive_points.push_back(v2);
						if (find(Norsum[v1].Consecutive_points.begin(), Norsum[v1].Consecutive_points.end(), v3) == Norsum[v1].Consecutive_points.end())
							Norsum[v1].Consecutive_points.push_back(v3);

						Norsum[v2].sum++;
						Norsum[v2].sumNormal += normal1;
						if (find(Norsum[v2].Consecutive_points.begin(), Norsum[v2].Consecutive_points.end(), v1) == Norsum[v2].Consecutive_points.end())
							Norsum[v2].Consecutive_points.push_back(v1);
						if (find(Norsum[v2].Consecutive_points.begin(), Norsum[v2].Consecutive_points.end(), v3) == Norsum[v2].Consecutive_points.end())
							Norsum[v2].Consecutive_points.push_back(v3);

						Norsum[v3].sum++;
						Norsum[v3].sumNormal += normal1;
						if (find(Norsum[v3].Consecutive_points.begin(), Norsum[v3].Consecutive_points.end(), v2) == Norsum[v3].Consecutive_points.end())
							Norsum[v3].Consecutive_points.push_back(v2);
						if (find(Norsum[v3].Consecutive_points.begin(), Norsum[v3].Consecutive_points.end(), v1) == Norsum[v3].Consecutive_points.end())
							Norsum[v3].Consecutive_points.push_back(v1);

						break;
					default:
						break;
					}

					faceCount++;
				}
			}
			else if (line[0] == '#') // ע�ͺ���
			{
			}
			else
			{
				// �������� ��ʱ������
			}
		}

		vertData = returnData(vertData,Norsum);
		
		return true;
	}

	vector<Vertex> laplace()
	{
		vector<Vertex> LavertData;
		SumNorStruct Norsum[40000];
		vec3 normal1;
		for (vector<GLuint>::size_type i = 0; i < temp_vertices.size(); ++i)
		{
			vec3 ave = vec3(0,0,0);
			int size = Norsum[i].Consecutive_points.size();
			for (int a = 0; a < size; ++a)
			{
				ave += temp_vertices[Norsum[i].Consecutive_points[a]];
			}
			
			ave /= size;
			vec3 minus = ave - temp_vertices[i];
			minus *= 0.7;
			temp_vertices[i] = temp_vertices[i] + minus;
		}

		GLint v1, v2, v3;
		for (int b = 0; b < vertice_index.size(); b++)
		{
			switch (b % 3)
			{
			case 0:
				v1 = vertice_index[b];
				break;
			case 1:
				v2 = vertice_index[b];
				break;
			case 2:
				v3 = vertice_index[b];

				normal1 = cross(temp_vertices[v2] - temp_vertices[v1], temp_vertices[v3] - temp_vertices[v1]);

				Norsum[v1].sum++;
				Norsum[v1].sumNormal += normal1;
				if (find(Norsum[v1].Consecutive_points.begin(), Norsum[v1].Consecutive_points.end(), v2) == Norsum[v1].Consecutive_points.end())
					Norsum[v1].Consecutive_points.push_back(v2);
				if (find(Norsum[v1].Consecutive_points.begin(), Norsum[v1].Consecutive_points.end(), v3) == Norsum[v1].Consecutive_points.end())
					Norsum[v1].Consecutive_points.push_back(v3);

				Norsum[v2].sum++;
				Norsum[v2].sumNormal += normal1;
				if (find(Norsum[v2].Consecutive_points.begin(), Norsum[v2].Consecutive_points.end(), v1) == Norsum[v2].Consecutive_points.end())
					Norsum[v2].Consecutive_points.push_back(v1);
				if (find(Norsum[v2].Consecutive_points.begin(), Norsum[v2].Consecutive_points.end(), v3) == Norsum[v2].Consecutive_points.end())
					Norsum[v2].Consecutive_points.push_back(v3);

				Norsum[v3].sum++;
				Norsum[v3].sumNormal += normal1;
				if (find(Norsum[v3].Consecutive_points.begin(), Norsum[v3].Consecutive_points.end(), v2) == Norsum[v3].Consecutive_points.end())
					Norsum[v3].Consecutive_points.push_back(v2);
				if (find(Norsum[v3].Consecutive_points.begin(), Norsum[v3].Consecutive_points.end(), v1) == Norsum[v3].Consecutive_points.end())
					Norsum[v3].Consecutive_points.push_back(v1);

				break;
			}
		}


		vector<Vertex> vertData;
		primaryVector.clear();

		vector<Vertex>& vertdata2 = returnData(vertData, Norsum);
		return(vertdata2);
	}

	vector<Vertex>& returnData(vector<Vertex>& vertData, SumNorStruct Norsum[40000])
	{
		Vertex vert;
		for (vector<GLuint>::size_type i = 0; i < temp_vertices.size(); ++i) //������˳�������շ�����
		{
			vec3 sumnormal = Norsum[i].sumNormal;
			GLint sum = Norsum[i].sum;

			sumnormal /= sum;

			vert.normal = sumnormal;
			vert.position = temp_vertices[i];
			primaryVector.push_back(vert);
		}

		for (vector<GLuint>::size_type i = 0; i < vertice_index.size(); ++i)
		{
			vert = primaryVector[vertice_index[i]];
			vertData.push_back(vert);
		}

		return vertData;
	}

};


#endif