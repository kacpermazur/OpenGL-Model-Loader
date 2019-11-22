#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

struct Model
{
	std::vector<unsigned int> vertexIndex, uvIndex, normalIndex, TESTvertIndex;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textCord;
	std::vector<glm::vec3> normals;
	
	std::vector<unsigned int> IV;
	std::vector<unsigned int> IUV;
	std::vector<unsigned int> IN;

	std::vector<glm::vec3> GetVerticesIndex()
	{
		std::vector<glm::vec3> vIndex;

		for (unsigned int i = 0; i < vertexIndex.size(); i++)
		{
			unsigned int index = vertexIndex[i];
			glm::vec3 item = vertices[index - 1];
			vIndex.push_back(item);
		}

		std::cout << "Cord Index: " << std::endl;
		Log(vIndex);

		return vIndex;
	}

	std::vector<glm::vec2> GetUVIndex()
	{
		std::vector<glm::vec2> vIndex;

		for (unsigned int i = 0; i < uvIndex.size(); i++)
		{
			unsigned int index = uvIndex[i];
			glm::vec2 item = textCord[index - 1];
			vIndex.push_back(item);
		}

		std::cout << "Texture Index: " << std::endl;
		Log(vIndex);

		return vIndex;
	}

	std::vector<glm::vec3> GetNormalIndex()
	{
		std::vector<glm::vec3> vIndex;

		for (unsigned int i = 0; i < normalIndex.size(); i++)
		{
			unsigned int index = normalIndex[i];
			glm::vec3 item = normals[index - 1];
			vIndex.push_back(item);
		}

		std::cout << "Normal Index: " << std::endl;
		Log(vIndex);

		return vIndex;
	}

	void Print()
	{
		std::cout << "Vertices: " << std::endl;
		Log(vertices);
		std::cout << "Uvs: " << std::endl;
		Log(textCord);
		std::cout << "Normals: " << std::endl;
		Log(normals);
		std::cout << "VertexIndex" << std::endl;
		Log(vertexIndex);
		std::cout << "UV Index" << std::endl;
		Log(uvIndex);
		std::cout << "Normal Index" << std::endl;
		Log(normalIndex);
	}

private:
	void Log(std::vector<glm::vec3>& target)
	{
		for (glm::vec3& vec : target)
		{
			std::cout << glm::to_string(vec) << std::endl;
		}
	}
	
	void Log(std::vector<glm::vec2>& target)
	{
		for (glm::vec2& vec : target)
		{
			std::cout << glm::to_string(vec) << std::endl;
		}
	}
	
	void Log(std::vector<unsigned>& target)
	{
		for (unsigned int& i : target)
		{
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}

};

class ModelLoader
{
public:
	
	static Model Load(const std::string& filePath)
	{
		Model model;
	
		std::string currentLine;
		std::ifstream stream(filePath);
		
		while(std::getline(stream, currentLine))
		{
			if(currentLine.length() > 2 && currentLine.substr(0,2) == "v ")
			{
				std::istringstream ss(currentLine.substr(2));
				glm::vec3 temp; ss >> temp.x; ss >> temp.y; ss >> temp.z;
				model.vertices.push_back(temp);
			}
			else if (currentLine.length() > 3 && currentLine.substr(0, 3) == "vt ")
			{
				std::istringstream ss(currentLine.substr(3));
				glm::vec2 temp; ss >> temp.x; ss >> temp.y;
				model.textCord.push_back(temp);
			}
			else if (currentLine.length() > 3 && currentLine.substr(0, 3) == "vn ")
			{
				std::istringstream ss(currentLine.substr(3));
				glm::vec3 temp; ss >> temp.x; ss >> temp.y; ss >> temp.z;
				model.normals.push_back(temp);
			}
			else if (currentLine.length() > 2 && currentLine.substr(0, 2) == "f ")
			{
				std::string faceLine = currentLine.substr(2);

				if(IsTriangulated(faceLine))
					ReadFaces(model, faceLine, 3);
				else
					ReadFaces(model, faceLine, 4);
			}
			
		}
		stream.close();


		return model;
	}
private:
	static bool IsTriangulated(std::string& line)
	{
		std::istringstream isStream(line);
		std::string face;
		int count = 0;
		
		while(isStream >> face)
			count += 1;

		return (count == 3)? true : false;
	}

	static void ReadFaces(Model& model, std::string& line, unsigned int facesPerLine)
	{
		std::vector<unsigned int> tempVert, tempNorm;
		
		std::istringstream ss(line);
		std::string face;
		unsigned int value;
		char avoid;

		std::cout << "======================================" << std::endl;
		std::cout << facesPerLine << std::endl;
		
		for (unsigned int i = 0; i < facesPerLine; i++)
		{
			ss >> face;
			std::istringstream faceStream(face);

			faceStream >> value; tempVert.push_back(value);
			faceStream >> avoid;
			faceStream >> value; model.uvIndex.push_back(value);
			faceStream >> avoid;
			faceStream >> value; tempNorm.push_back(value);
		}
		/*
		std::cout << "---------------------------------------------------------------------" << std::endl;
		std::cout << tempVert[0] << std::endl;
		std::cout << tempVert[1] << std::endl;
		std::cout << tempVert[2] << std::endl;
		std::cout << tempVert[3] << std::endl;
		
		std::cout << "---------------------------------------------------------------------" << std::endl;
		*/
		SetVertexIndex(model, tempVert, facesPerLine);
		SetNormalIndex(model, tempNorm, facesPerLine);
	}

	static void SetVertexIndex(Model& model, std::vector<unsigned int>& tempVector, unsigned int& count)
	{
		if(count == 4)
		{
			model.vertexIndex.push_back(tempVector[0]);
			model.vertexIndex.push_back(tempVector[2]);
			model.vertexIndex.push_back(tempVector[1]);

			model.vertexIndex.push_back(tempVector[0]);
			model.vertexIndex.push_back(tempVector[3]);
			model.vertexIndex.push_back(tempVector[2]);
		}
		else if (count == 3)
		{
			model.vertexIndex.push_back(tempVector[0]);
			model.vertexIndex.push_back(tempVector[1]);
			model.vertexIndex.push_back(tempVector[2]);
		}
		else
		{
			std::cout << "ERROR: Unable to Set VertexIndex!" << std::endl;
		}
	}

	static void SetNormalIndex(Model& model, std::vector<unsigned int>& tempVector, unsigned int& count)
	{
		if (count == 4)
		{
			model.normalIndex.push_back(tempVector[0]);
			model.normalIndex.push_back(tempVector[2]);
			model.normalIndex.push_back(tempVector[1]);

			model.normalIndex.push_back(tempVector[0]);
			model.normalIndex.push_back(tempVector[3]);
			model.normalIndex.push_back(tempVector[2]);
		}
		else if (count == 3)
		{
			model.normalIndex.push_back(tempVector[0]);
			model.normalIndex.push_back(tempVector[2]);
			model.normalIndex.push_back(tempVector[1]);
		}
		else
		{
			std::cout << "ERROR: Unable to Set NormalIndex!" << std::endl;
		}
	}
};