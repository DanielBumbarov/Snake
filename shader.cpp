#include <stdio.h>
#include <string>
#include <vector>

#include <iostream>
#include <fstream>

#include <algorithm>

#include <stdlib.h>
#include <string.h>

#include <GLEW/glew.h>

#include "shader.hpp"
using namespace std;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";

		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();

	}
	else{
		printf("Cant open %s.\n", vertex_file_path);
		return 0;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int logInfo;

	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShader, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShader);

	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &logInfo);
	if ( logInfo > 0 ){
		std::vector<char> VertexShaderErrorMessage(logInfo+1);
		glGetShaderInfoLog(VertexShader, logInfo, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShader, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &logInfo);
	if ( logInfo > 0 ){
		std::vector<char> FragmentShaderErrorMessage(logInfo+1);
		glGetShaderInfoLog(FragmentShader, logInfo, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShader);
	glAttachShader(ProgramID, FragmentShader);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &logInfo);
	if ( logInfo > 0 ){
		std::vector<char> ProgramErrorMessage(logInfo+1);
		glGetProgramInfoLog(ProgramID, logInfo, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShader);
	glDetachShader(ProgramID, FragmentShader);
	
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return ProgramID;
}


