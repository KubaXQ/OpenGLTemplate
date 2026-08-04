#pragma once

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	// startposition - gdzie kamera ma stac na starcie
	// startup - "gora" swiata, zawsze stale [0,1,0] (sufit)
	// startyaw/startpitch - poczatkowy kat patrzenia lewo/prawo i gora/dol
	// startMovementSpeed - predkosc chodzenia (jednostki/sekunde)
	// startTurnSpeed - czulosc myszy (mnoznik obrotu)
	Camera(glm::vec3 startposition, glm::vec3 startup, GLfloat startyaw, GLfloat startpitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed);

	// obsluga WASD - przesuwa position w oparciu o wektory front/right i deltaTime (stala predkosc niezaleznie od FPS)
	void keyControl(bool* keys, GLfloat deltaTime);
	// obsluga myszy - zmienia yaw/pitch na podstawie ruchu myszy od ostatniej klatki
	void mouseControl(GLfloat xChange, GLfloat yChange);

	// buduje macierz view (glm::lookAt) na podstawie aktualnej pozycji i kierunku patrzenia
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;	// gdzie kamera aktualnie jest w swiecie
	glm::vec3 front;		// kierunek patrzenia (w ktora strone "przod")
	glm::vec3 up;			// "gora" WZGLEDEM kamery - moze sie roznic od worldUp gdy kamera jest przechylona
	glm::vec3 right;		// "prawo" kamery, potrzebne do strafe'owania (A/D)
	glm::vec3 worldUp;		// stala "gora" calego swiata (sufit) - punkt odniesienia do liczenia right/up

	GLfloat yaw;	// kat obrotu lewo/prawo (rotacja wokol osi Y)
	GLfloat pitch;	// kat patrzenia gora/dol (ograniczony do -89/+89 zeby uniknac gimbal lock)

	GLfloat movementSpeed;	// mnoznik predkosci ruchu
	GLfloat turnSpeed;		// mnoznik czulosci myszy

	// przelicza front/right/up na nowo na podstawie aktualnego yaw/pitch
	// wolane zawsze po zmianie kata patrzenia (w konstruktorze i w mouseControl)
	void update();


};