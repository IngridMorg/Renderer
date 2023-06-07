#pragma once
#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//inspo: https://learnopengl.com/Getting-started/Shaders

class Shader{
public:
	//program/shader ID
	unsigned int ID;

	//constructor
	Shader(const char* vertexPath, const char* fragmentPath) {
		//step 1
		//retrieve the vertex/fragment source code from the filepath
		std::string vertexCode;
		std::string fragmentCode;
		//ifstream - input stream class specifically designed for file operations
		//https://cplusplus.com/reference/fstream/ifstream/
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//allow for ifstream objects throwing any exceptions/tantrums
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			//open the files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//read the contentsof the files buffer into the streams (i hate stream io, alas)
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//close our file handlers
			vShaderFile.close();
			fShaderFile.close();
			//convert the stream into a string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR:: Shader file has not been successfully read from file\n FAILURE IN:: Shader.h , Shader()" << std::endl;
		}
		//set up some consts
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//step 2 0_0
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		//vertex shader compilation & linking
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//print/display any compile errors generated
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR:: Vertex shader compilation has failed\n FAILURE IN:: Shader.h, Shader()" << std::endl;
		};

		//fragment shader compilation & linking
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		//print/display any compile errors generated
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR:: Fragment shader compilation has failed\n FAILURE IN:: Shader.h, Shader()" << std::endl;
		};

		//shader program tings
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//print any linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR:: Shader program files have failed when linking\n FAILURE IN:: Shader.h, Shader()" << std::endl;
		}
		//delete the shaders because now we can reference them through the program (hell yeah)
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//activate shader
	void activate();
	//utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

};

