#include "../lib/glew/glew.h"
#include "../include/screen.h"

std::shared_ptr<Screen> Screen::mInstance = nullptr;

Screen::Screen() {
	mWindow = nullptr;
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);
}

Screen::~Screen() {
	Close();
	glfwTerminate();
}

void Screen::Open(uint16 width, uint16 height, bool fullscreen) {
	// Si había una ventana abierta, la cerramos
	Close();

	// Abrimos la ventana
	mWindow = glfwCreateWindow(width, height, "", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);
	glewInit();
	mWidth = width;
	mHeight = height;

	// Inicializamos temporizador
	mLastTime = glfwGetTime();
	mElapsed = 0;
}

void Screen::Close() {
	glfwDestroyWindow(mWindow);
	mWindow = nullptr;
}

bool Screen::ShouldClose() const {
	return glfwWindowShouldClose(mWindow) == GL_TRUE;
}

void Screen::SetTitle(const std::string& title) {
	glfwSetWindowTitle(mWindow, title.c_str());
}

void Screen::Refresh() {
	double mx, my;
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	glfwGetCursorPos(mWindow, &mx, &my);
	mMouseX = int32(mx);
	mMouseY = int32(my);
	mElapsed = glfwGetTime() - mLastTime;
	mLastTime = glfwGetTime();
}

uint16 Screen::GetDesktopWidth() const {
	return uint16(glfwGetVideoMode(glfwGetPrimaryMonitor())->width);
}

uint16 Screen::GetDesktopHeight() const {
	return uint16(glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
}

void Screen::ShowMouse(bool show) {
	glfwSetInputMode(mWindow, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Screen::MoveMouse(int x, int y) {
	glfwSetCursorPos(mWindow, x, y);
}

int Screen::GetMouseX() {
	return mMouseX;
}

int Screen::GetMouseY() {
	return mMouseY;
}

bool Screen::IsMousePressed(int button) const {
	return glfwGetMouseButton(mWindow, button) == GLFW_PRESS;
}

bool Screen::IsKeyPressed(int key) const {
	return glfwGetKey(mWindow, key) == GLFW_PRESS;
}
