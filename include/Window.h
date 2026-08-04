#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	// tworzy okno GLFW, kontekst OpenGL, wczytuje GLEW, wlacza depth test, rejestruje callbacki inputu
	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; };
	GLfloat getBufferheight() { return bufferHeight; };

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); };

	// zwraca wskaznik na tablice stanu klawiszy (keys[KOD_KLAWISZA] = true/false)
	bool* getKeys() { return keys; }
	// zwraca zmiane pozycji myszy w X od ostatniego odczytu i ZERUJE ja (kazda "porcja" ruchu liczona raz)
	GLfloat getXChange();
	// jw. dla osi Y
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }


	~Window();
private:

	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];	// "checklista" stanu wszystkich klawiszy - true = wcisniety

	GLfloat lastX;		// ostatnia znana pozycja myszy X (do liczenia roznicy)
	GLfloat lastY;
	GLfloat xChange;	// ile mysz przesunela sie od ostatniego odczytu
	GLfloat yChange;
	bool mousedFirstMoved;	// flaga zabezpieczajaca przed "skokiem" kamery przy pierwszym ruchu myszy po starcie

	// callbacki GLFW musza byc static (GLFW to biblioteka C, nie zna metod klas C++)
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

	// rejestruje powyzsze funkcje jako callbacki w GLFW
	void createCallBacks();
};