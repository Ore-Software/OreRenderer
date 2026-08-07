#pragma once

#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow *m_ID;

public:
	Window(unsigned int width, unsigned int height, const std::string &title, bool isFullscreen);
	~Window();

	GLFWwindow *GetID() const;

private:
	int Create(unsigned int width, unsigned int height, const std::string &title, bool isFullscreen);
	void Destroy();
};