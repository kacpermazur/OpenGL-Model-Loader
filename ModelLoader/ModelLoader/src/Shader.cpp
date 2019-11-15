#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_BindID(0)
{
	ShaderProgramSoruce soruce = ParseShader(filepath);
	m_BindID = CreateShader(soruce.VertexSource, soruce.FragmentSource);

	std::cout << soruce.VertexSource << std::endl;
	std::cout << soruce.FragmentSource << std::endl;
}

Shader::~Shader()
{
	glDeleteProgram(m_BindID);
}

void Shader::bind() const
{
	glUseProgram(m_BindID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

ShaderProgramSoruce Shader::ParseShader(const std::string& filepath)
{
	enum class Shader
	{
		INVALID = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream stringStream[2];
	
	Shader type = Shader::INVALID;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = Shader::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = Shader::FRAGMENT;
			}
		}
		else
		{
			stringStream[(int)type] << line << '\n';
		}
	}

	return { stringStream[0].str(), stringStream[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	return ShaderErrorCheck(id, type);
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::ShaderErrorCheck(unsigned int& id, unsigned int& type)
{
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Shader Compiler Error: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	
	return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_BindID, name.c_str());
	if (location == -1)
	{
		std::cout << "Uniform Error: " << name << " wasnt found!" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}
