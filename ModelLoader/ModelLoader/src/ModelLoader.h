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
	std::vector<unsigned int> vertexIndex, uvIndex, normalIndex;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	void Print()
	{
		std::cout << "Vertices: " << std::endl;
		Log(vertices);
		std::cout << "Uvs: " << std::endl;
		Log(uvs);
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
				model.uvs.push_back(temp);
			}
			else if (currentLine.length() > 3 && currentLine.substr(0, 3) == "vn ")
			{
				std::istringstream ss(currentLine.substr(3));
				glm::vec3 temp; ss >> temp.x; ss >> temp.y; ss >> temp.z;
				model.normals.push_back(temp);
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
		std::istringstream ss(line);
		std::string face;
		unsigned int value;
		char avoid;
		
		for (unsigned int i = 0; i < facesPerLine; i++)
		{
			ss >> face;
			std::istringstream faceStream(face);

			faceStream >> value; model.vertexIndex.push_back(value);
			faceStream >> avoid;
			faceStream >> value; model.uvIndex.push_back(value);
			faceStream >> avoid;
			faceStream >> value; model.normalIndex.push_back(value);
		}
	}
};