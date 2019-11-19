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
				std::istringstream ss(currentLine.substr(2));
				
				ss >> temp.x; ss >> temp.y; ss >> temp.z;
			}
			
		}
		stream.close();
		return model;
	}
};