#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include<GL/glew.h>

class Shader
{
public:
	Shader();

	// kompiluje shader z gotowych stringow kodu GLSL (bez czytania plikow)
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	// wczytuje kod GLSL z plikow .vert/.frag, potem kompiluje
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	// wczytuje caly plik tekstowy do jednego stringa (linijka po linijce)
	std::string ReadFile(const char* fileLocation);

	// gettery lokalizacji uniformow - pobrane raz po zlinkowaniu shadera
	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();


	// aktywuje ten program shaderow (glUseProgram) - wolane przed rysowaniem
	void UseShader();

	// zwalnia program shaderow z pamieci GPU
	void ClearShader();


	~Shader();

private:
	GLuint shaderID, uniformModel, uniformProjection, uniformView;

	// tworzy program, kompiluje i linkuje oba shadery, pobiera lokalizacje uniformow
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	// kompiluje pojedynczy shader (vertex lub fragment) i podpina go do programu
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};