#ifndef UGINE_SCREEN_H
#define UGINE_SCREEN_H

#include "string.h"
#include "types.h"

#define MOUSE_LEFT       0
#define MOUSE_RIGHT      1
#define MOUSE_MIDDLE     2

class Screen {
public:
	static const std::shared_ptr<Screen>& Instance();

	// Screen
	void Open(uint16 width, uint16 height, bool fullscreen);
	void Close();
	bool ShouldClose() const;
	void SetTitle(const std::string& title);
	void Refresh();
	uint16 GetWidth() const { return mWidth; }
	uint16 GetHeight() const { return mHeight; }
	uint16 GetDesktopWidth() const;
	uint16 GetDesktopHeight() const;
	float GetElapsed() const { return mElapsed; }

	// Mouse
	void ShowMouse(bool show);
	void MoveMouse(int x, int y);
	int GetMouseX();
	int GetMouseY();
	bool IsMousePressed(int button) const;

	// Keyboard
	bool IsKeyPressed(int key) const;
protected:
	Screen();
	~Screen();
	static void Delete(Screen* s) { delete s; }
private:
	static std::shared_ptr<Screen> mInstance;

	GLFWwindow* mWindow;
	uint16 mWidth, mHeight;
	int32 mMouseX, mMouseY;
	float mLastTime, mElapsed;
};

inline const std::shared_ptr<Screen>& Screen::Instance() {
	if ( !mInstance )
		mInstance = std::shared_ptr<Screen>(new Screen(), Delete);
	return mInstance;
}

#endif // UGINE_SCREEN_H
