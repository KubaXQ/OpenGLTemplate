#pragma once
#include <GL/glew.h>

class Mesh
{

public:
	Mesh();

	// tworzy VAO/VBO/IBO dla danego zestawu wierzcholkow i indeksow (index draw)
	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	// binduje VAO/IBO i rysuje ten mesh (glDrawElements)
	void RenderMesh();
	// zwalnia bufory z pamieci GPU
	void ClearMesh();

	~Mesh();
private:
	GLuint VAO, VBO, IBO;	// kazdy Mesh ma WLASNE bufory - pozwala trzymac wiele roznych ksztaltow naraz
	GLsizei indexCount;		// ile indeksow ma ten mesh (potrzebne do glDrawElements)


};