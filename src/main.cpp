#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f;	// przelicznik stopnie -> radiany (GLM/funkcje trygonometryczne chca radianow)


Window mainWindow;
std::vector<Mesh*> meshList;		// lista wszystkich obiektow (ksztaltow) na scenie
std::vector<Shader> shaderList;	// lista programow shaderow dostepnych do uzycia
Camera camera;

GLfloat deltaTime = 0.0f;	// czas jaki minal od poprzedniej klatki - do ruchu niezaleznego od FPS
GLfloat lastTime = 0.0f;	// czas z poprzedniej klatki, do liczenia deltaTime

//Vertex Shader
static const char* vShader = "Shaders/shader.vert";


//Fragment shader
static const char* fShader = "Shaders/shader.frag";

// tworzy geometrie sceny (na razie 2 identyczne obiekty) i dodaje je do meshList
void CreateObjects() {

	unsigned int indices[]{
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2

	};

	GLfloat verticies[] = {
		-1.0f,-1.0f,0.0f,
		0.0f,-1.0f,1.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(verticies, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(verticies, indices, 12, 12);
	meshList.push_back(obj2);
}


// wczytuje i kompiluje shader z plikow .vert/.frag, dodaje do shaderList
void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {

	// 1. okno i kontekst OpenGL
	mainWindow = Window(800, 600);
	mainWindow.Initialise();


	// 2. geometria i shadery
	CreateObjects();
	CreateShaders();

	// 3. kamera: pozycja startowa, "gora" swiata, yaw -90 (patrz przed siebie), pitch 0, predkosc ruchu 5, czulosc myszy 1
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	// macierz projekcji - liczona RAZ przed petla, bo sie nie zmienia co klatke (FOV, proporcje, near/far plane)
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferheight(), 0.1f, 100.0f);



	while (!mainWindow.getShouldClose()) {

		// delta time - ile realnego czasu minelo od poprzedniej klatki
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();	// odpala callbacki input (handleKeys/handleMouse w Window), jesli cos sie zmienilo

		// ruch i rozgladanie sie kamery na podstawie inputu z tej klatki
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// czyscimy tez bufor glebi (depth test)

		shaderList[0].UseShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();
		uniformView = shaderList[0].getViewLocation();


		// --- obiekt 1: wlasna macierz model (pozycja/rotacja/skala), potem rysowanie ---
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));	// odsuniecie od kamery, zeby bylo widoczne
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f,1.0f,0.0f ));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// view liczone na nowo co klatke - kamera mogla sie poruszyc/obrocic
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		// --- obiekt 2: osobna macierz model (inna pozycja Y), ten sam shader i projection/view ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}