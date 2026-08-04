#include "Window.h"


Window::Window() {
	width = 800;
	height = 600;

	// na starcie zaden klawisz nie jest wcisniety
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	xChange = 0.0f;
	yChange = 0.0f;
}
int Window::Initialise()
{
	if (!glfwInit()) {
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// wymuszamy OpenGL 3.3 core profile (bez przestarzalych funkcji)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Okno", NULL, NULL);

	if (!mainWindow) {
		printf("window failed");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	//handle key mouse input
	createCallBacks();

	// ukrywa kursor i "zamyka" go w oknie - standardowe zachowanie kamery FPS
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// wlacza test glebi - GPU rysuje tylko to co najblizej kamery w danym pikselu
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);


	// zapisuje w GLFW wskaznik na ten obiekt Window, zeby statyczne callbacki
	// (handleKeys/handleMouse) mogly go pozniej odzyskac i modyfikowac jego pola
	glfwSetWindowUserPointer(mainWindow, this);
}

// rejestruje funkcje handleKeys/handleMouse jako callbacki wywolywane automatycznie przez GLFW
void Window::createCallBacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

// zwraca zmiane pozycji myszy w X i zeruje ja, zeby nie zostala "zuzyta" wielokrotnie w kolejnych klatkach
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

// callback GLFW - wywolywany automatycznie przy kazdym wcisnieciu/puszczeniu klawisza
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	// odzyskujemy wskaznik na konkretny obiekt Window zapisany wczesniej w Initialise()
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// ESC zamyka okno/program
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;

		}
	}
}

// callback GLFW - wywolywany automatycznie przy kazdym ruchu myszy
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// pierwszy ruch myszy po starcie - zamiast liczyc roznice (co dalo by falszywy skok),
	// po prostu zapamietaj aktualna pozycje jako punkt startowy
	if (theWindow->mousedFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mousedFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;	// odwrocone bo os Y ekranu rosnie w dol, a chcemy "w gore = dodatnio"

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}