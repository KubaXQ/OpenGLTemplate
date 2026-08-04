#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 startposition, glm::vec3 startup, GLfloat startyaw, GLfloat startpitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed)
{
	position = startposition;
	worldUp = startup;
	yaw = startyaw;
	pitch = startpitch;
	front = glm::vec3(0.0f, 0.0f, -0.1f);	// wartosc startowa, i tak zostanie nadpisana przez update() ponizej

	movementSpeed = startMovementSpeed;
	turnSpeed = startTurnSpeed;

	// przelicz front/right/up na podstawie startowego yaw/pitch, zanim cokolwiek narysujemy
	update();
}

// wywolywane co klatke w main - przesuwa kamere w oparciu o wcisniete klawisze
void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	// predkosc = predkosc bazowa * czas od ostatniej klatki -> ruch niezalezny od FPS
	GLfloat velocioty = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocioty;		// do przodu, w kierunku patrzenia
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocioty;		// do tylu
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocioty;		// strafe w lewo
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocioty;		// strafe w prawo
	}
}

// wywolywane co klatke w main - obraca kamere na podstawie roznicy pozycji myszy od ostatniej klatki
void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	// czulosc myszy - skaluje surowy ruch myszki na kat obrotu
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	// limit patrzenia gora/dol - bez tego przy +-90 stopni kamera "przekreca sie" (gimbal lock)
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	// kat sie zmienil, wiec trzeba przeliczyc wektory kierunkowe na nowo
	update();
}

// zwraca macierz widoku (view) - pozycja kamery, punkt na ktory patrzy (position+front), oraz jej "gora"
glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);

}

// przelicza front/right/up na podstawie aktualnego yaw i pitch (trygonometria)
// front.y zalezy tylko od pitch (patrzenie gora/dol nie zalezy od tego, w ktora strone jestesmy skreceni)
// front.x/front.z zaleza od kombinacji yaw i pitch (kierunek w plaszczyznie pozioma * ile "do przodu" ze wzgledu na pitch)
void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);		// interesuje nas tylko kierunek, nie dlugosc wektora

	// right = kierunek prostopadly do "przodu" i "sufitu swiata" (cross product)
	right = glm::normalize(glm::cross(front, worldUp));
	// prawdziwe up kamery = prostopadle do "przodu" i "prawa" (moze sie roznic od worldUp przy przechyleniu)
	up = glm::normalize(glm::cross(right, front));

}

Camera::~Camera()
{



}