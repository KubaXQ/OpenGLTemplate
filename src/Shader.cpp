#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);

}

// wczytuje oba pliki GLSL z dysku, konwertuje na const char* i kompiluje
void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

// otwiera plik tekstowy i skleja go linijka po linijce w jeden string (dokladnie tak jak
// wczesniej kod shadera byl pisany na sztywno w C++ jako string)
std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesnt exist.", fileLocation);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("error creating shader program");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	// laczy vertex i fragment shader w jeden dzialajacy program
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		printf("Error Linking program: '%s'\n", elog);
		return;
	}

	// dodatkowa walidacja czy program da sie uruchomic w aktualnym stanie OpenGL
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		printf("Error Validating program: '%s'\n", elog);
		return;
	}


	// pobranie lokalizacji uniformow - MUSI byc po zlinkowaniu programu, robimy to raz
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");

}

// kompiluje pojedynczy shader (vertex lub fragment) z kodu GLSL i podpina go do programu
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{

	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
		printf("Error Compiling the %d shader: '%s'\n", shaderType, elog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::getModelLocation()
{
	return uniformModel;
}
GLuint Shader::getViewLocation()
{
	return uniformView;
}


// aktywuje ten program shaderow - musi byc wywolane przed wyslaniem uniformow i rysowaniem
void Shader::UseShader()
{
	glUseProgram(shaderID);
}

// zwalnia program shaderow z pamieci GPU (np. przy zamykaniu programu)
void Shader::ClearShader()
{
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}





Shader::~Shader()
{
	ClearShader();
}