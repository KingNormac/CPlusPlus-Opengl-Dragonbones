#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "Shader.h"

	std::string vcode;
	std::string fcode; 

	void douptheshader() {

		vcode = "";
		vcode +="#version 120           \n";
		vcode +="attribute vec2 coord2d; \n";
		vcode +="attribute vec2 texcoord;\n";
		vcode +="varying vec2 f_texcoord;\n";
		vcode +="uniform mat4 mvp;\n";
		vcode +="void main(void) {\n";
		vcode +="gl_Position = mvp * vec4(coord2d, 0.0, 1.0);\n";
		vcode +="f_texcoord = texcoord;\n";
		vcode +="}\n";

		fcode = "";
		fcode += "#version 120           \n";
		fcode +="varying vec2 f_texcoord;    \n";
		fcode +="uniform sampler2D mytexture;\n";
		fcode +="void main(void) {\n";
		fcode +="vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);\n";
		fcode +="gl_FragColor = texture2D(mytexture, flipped_texcoord);\n";
		fcode +="}\n";
	}



GLuint LoadShaders(){

	douptheshader();
	printf("Compiling Shader \n");
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const* VertexSourcePointer = vcode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		if(VertexShaderErrorMessage[0] > 0)
			printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	char const * FragmentSourcePointer = fcode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		if(FragmentShaderErrorMessage[0] > 0)
			printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		if(ProgramErrorMessage[0] > 0)
			printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}



