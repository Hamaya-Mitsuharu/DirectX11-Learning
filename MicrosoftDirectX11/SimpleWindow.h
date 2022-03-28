#pragma once
#include<windows.h>

class SimpleWindow {
public:
	SimpleWindow();
	~SimpleWindow(void);

	bool CreateSimpleWindow(LPCTSTR windowName, HINSTANCE hInstance);
	bool UpdateWindowMessage(void);

	bool IsQuitMessage(void) { return _isQuitMessage; };
	MSG  GetMessage(void) { return _msg; };
	HWND GetWindowHandle(void) { return _hwnd; };
	UINT GetWidth(void) { return _width; };
	UINT GetHeight(void) { return _height; };
private:
	HWND _hwnd;
	bool _isQuitMessage;
	MSG _msg;
	UINT _width;
	UINT _height;
};