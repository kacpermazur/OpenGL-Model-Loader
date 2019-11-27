#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <unordered_map>



#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex
{
	glm::vec3 positon;
	glm::vec2 texCoord;
	glm::vec3 normals;

	
	bool operator==(const Vertex& other) const {
		return positon == other.positon && texCoord == other.texCoord && normals == other.normals;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		
		void hash_combine(::size_t& seed, const unsigned int& hash) const
		{
			seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		
		::size_t operator()(Vertex const& vertex) const
		{
			auto hasher = hash<glm::vec3>{};
			auto hashed_pos = hasher(vertex.positon);
			auto hashed_nor = hasher(vertex.normals);
			auto hasher2 = hash<glm::vec2>{};
			auto hashed_tex = hasher2(vertex.texCoord);

			::size_t seed = 0;
			hash_combine(seed, hashed_pos);
			hash_combine(seed, hashed_nor);
			hash_combine(seed, hashed_tex);
			return seed;
		}
	};
}

class MeshLoader
{
public:
	static std::vector<Vertex> Load(const std::string& filePath, std::vector<unsigned int>& ref)
	{
		std::vector<unsigned int> vertIndex, texCoordIndex, normalIndex;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		
		std::string currentLine;
		std::ifstream stream(filePath);
		std::vector<Vertex> src;
		
		while(std::getline(stream, currentLine))
		{
			
			if(currentLine.length() > 2 && currentLine.substr(0,2) == "v ")
			{
				std::istringstream ss(currentLine.substr(2));
				glm::vec3 temp; ss >> temp.x; ss >> temp.y; ss >> temp.z;
				vertices.push_back(temp);
			}
			else if (currentLine.length() > 3 && currentLine.substr(0, 3) == "vt ")
			{
				std::istringstream ss(currentLine.substr(3));
				glm::vec2 temp; ss >> temp.x; ss >> temp.y;
				texCoords.push_back(temp);
			}
			else if (currentLine.length() > 3 && currentLine.substr(0, 3) == "vn ")
			{
				std::istringstream ss(currentLine.substr(3));
				glm::vec3 temp; ss >> temp.x; ss >> temp.y; ss >> temp.z;
				normals.push_back(temp);
			}
			else if (currentLine.length() > 2 && currentLine.substr(0, 2) == "f ")
			{
				std::string faceLine = currentLine.substr(2);

				if(IsTriangulated(faceLine))
					ReadFaces(vertIndex, texCoordIndex, normalIndex, faceLine, 3);
				else
					ReadFaces(vertIndex, texCoordIndex, normalIndex, faceLine, 4);
			}
			
		}
		stream.close();

		auto pos = GenrateNewValues(vertices, vertIndex);
		auto tex = GenrateNewValues(texCoords, texCoordIndex);
		auto nor = GenrateNewValues(normals, normalIndex);

		//Log(pos);
		//Log(tex);
		//Log(nor);
		auto newValues = GenerateVertices(pos, nor);
		std::cout << "OLD VALUES: " << newValues.size() << std::endl;

		ref = DuplicationCheck(newValues);
		AddTexCoord(newValues, texCoords);

		/*for(auto& a: newValues)
		{
			std::cout << "New VALUES: " << glm::to_string(a.positon) << std::endl;
		}
		for (auto& a : newValues)
		{
			std::cout << "New VALUES: " << glm::to_string(a.texCoord) << std::endl;
		}
		for (auto& a : newValues)
		{
			std::cout << "New VALUES: " << glm::to_string(a.normals) << std::endl;
		}*/
		
		return newValues;
	}

private:
	static void Log(std::vector<glm::vec3>& target)
	{
		for (glm::vec3& vec : target)
		{
			std::cout << glm::to_string(vec) << std::endl;
		}
	}

	static void Log(std::vector<glm::vec2>& target)
	{
		for (glm::vec2& vec : target)
		{
			std::cout << glm::to_string(vec) << std::endl;
		}
	}

	static void Log(std::vector<unsigned>& target)
	{
		for (unsigned int& i : target)
		{
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}

	static void AddTexCoord(std::vector<Vertex>& target, std::vector<glm::vec2>& textCoord)
	{
		for (int i = 0; i < target.size(); i++)
		{
			target[i].texCoord = textCoord[i];
		}
	}
	
	static std::vector<unsigned int> DuplicationCheck(std::vector<Vertex>& target)
	{
		std::unordered_map<Vertex, unsigned int> uniqueVertices = {};

		std::vector<Vertex> tempVerices = {};
		std::vector<unsigned int> tempIndices = {};

		for (Vertex& vert : target)
		{
			Vertex vertex = {};

			vertex.positon = vert.positon;
			vertex.texCoord = vert.texCoord;
			vertex.normals = vert.normals;

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = tempVerices.size();
				tempIndices.push_back(tempVerices.size());
				tempVerices.push_back(vertex);
				
			}
			else
			{
				tempIndices.push_back(uniqueVertices.at(vertex));
			}
		}

		std::cout << "NEW VERTICES: " << tempVerices.size() << std::endl;
		std::cout << "NEW INDICES: " << tempIndices.size() << std::endl;
		//Log(tempIndices);

		/*for(auto& a : tempVerices)
		{

			std::cout << "TEMP VERTICES: " << glm::to_string(a.positon) << std::endl;
		}*/

		target = tempVerices;
		return tempIndices;
		
	}
	
	static std::vector<Vertex> GenerateVertices(std::vector<glm::vec3>& pos, std::vector<glm::vec3>& normals)
	{
		std::cout << "Pos Size: " << pos.size() << "nor Size: " << normals.size() << std::endl;
		std::vector<Vertex> vertices;


		for (int i = 0; i < pos.size(); i++)
		{
			Vertex vertex = {};

			vertex.positon = (pos[i]);
			vertex.normals = (normals[i]);

			vertices.push_back(vertex);
		}

		return vertices;
	}
	
	static std::vector<glm::vec3> GenrateNewValues(std::vector<glm::vec3>& target, std::vector<unsigned int>& indices)
	{
		std::vector<glm::vec3> newValues;

		for (unsigned int i = 0; i < indices.size(); i++)
		{
			unsigned int index = indices[i];
			glm::vec3 item = target[index - 1];
			newValues.push_back(item);
		}

		return newValues;
	}

	static std::vector<glm::vec2> GenrateNewValues(std::vector<glm::vec2>& target, std::vector<unsigned int>& indices)
	{
		std::vector<glm::vec2> newValues;

		for (unsigned int i = 0; i < indices.size(); i++)
		{
			unsigned int index = indices[i];
			glm::vec2 item = target[index - 1];
			newValues.push_back(item);
		}

		return newValues;
	}
	
	static bool IsTriangulated(std::string& line)
	{
		std::istringstream isStream(line);
		std::string face;
		int count = 0;

		while (isStream >> face)
			count += 1;

		return (count == 3) ? true : false;
	}
	
	static void ReadFaces(std::vector<unsigned int>& posIndex, std::vector<unsigned int>& texCoordIndex,
		std::vector<unsigned int>& normIndex, std::string& line, unsigned int facesPerLine)
	{
		std::vector<unsigned int> tempVert, tempNorm;

		std::istringstream ss(line);
		std::string face;
		unsigned int value;
		char avoid;

		for (unsigned int i = 0; i < facesPerLine; i++)
		{
			ss >> face;
			std::istringstream faceStream(face);

			faceStream >> value; tempVert.push_back(value);
			faceStream >> avoid;
			faceStream >> value; texCoordIndex.push_back(value);
			faceStream >> avoid;
			faceStream >> value; tempNorm.push_back(value);
		}

		SetIndex(posIndex, tempVert, facesPerLine);
		SetIndex(normIndex, tempNorm, facesPerLine);
	}

	static void SetIndex(std::vector<unsigned int>& target, std::vector<unsigned int>& tempVector, unsigned int& count)
	{
		if (count == 4)
		{
			target.push_back(tempVector[0]);
			target.push_back(tempVector[1]);
			target.push_back(tempVector[2]);

			target.push_back(tempVector[0]);
			target.push_back(tempVector[2]);
			target.push_back(tempVector[3]);
		}
		else if (count == 3)
		{
			target.push_back(tempVector[0]);
			target.push_back(tempVector[1]);
			target.push_back(tempVector[2]);
		}
		else
		{
			std::cout << "ERROR: Unable to Set VertexIndex!" << std::endl;
		}
	}
};