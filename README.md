# OpenGL Template

A simple OpenGL template written in **C++** using:

- OpenGL
- GLFW
- GLEW (Static)
- GLM
- CMake

## Features

- CMake project
- Camera class
- Mesh class
- Shader class
- Automatic shader copying after build
- Static GLEW linking

## Requirements

- Visual Studio 2022
- CMake 3.20 or newer

---

# Build

## Option 1 - Visual Studio (Recommended)

Clone the repository:

```bash
git clone https://github.com/TWOJ_LOGIN/OpenGLTemplate.git
```

Open the project folder in **Visual Studio 2022**:

```
File -> Open -> Folder
```

Select the folder containing `CMakeLists.txt`.

Visual Studio will automatically configure the CMake project.

Build the project:

```
Build -> Build All
```

Run the executable from Visual Studio.

---

## Option 2 - Command Line (CMake)

Clone the repository:

```bash
git clone https://github.com/TWOJ_LOGIN/OpenGLTemplate.git
```

Enter the project directory:

```bash
cd OpenGLTemplate
```

Create a build directory:

```bash
mkdir build
cd build
```

Generate the build files:

```bash
cmake ..
```

Build the project:

**Release**

```bash
cmake --build . --config Release
```

**Debug**

```bash
cmake --build . --config Debug
```

---

## Project Structure

```
OpenGLTemplate/
│
├── src/
├── include/
├── Shaders/
├── assets/
├── External_Libs/
├── CMakeLists.txt
└── README.md
```

---

## License

This project is intended for learning purposes.
