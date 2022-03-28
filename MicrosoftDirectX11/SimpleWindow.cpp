#include "SimpleWindow.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

SimpleWindow::SimpleWindow() : _hwnd(NULL), _isQuitMessage(false), _msg( {0} ), _width(0), _height(0){

}

SimpleWindow::~SimpleWindow(void) {

}

bool SimpleWindow::CreateSimpleWindow(LPCTSTR windowName, HINSTANCE hInstance) {
	WNDCLASS winc;
	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("テストクラス名");

	if (!RegisterClass(&winc)) return false;

	_hwnd = CreateWindow(
		TEXT("テストクラス名"), windowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		hInstance, NULL
	);

	if (_hwnd == NULL) return false;
	
	RECT rc;
	GetClientRect(_hwnd, &rc);
	_width = rc.right - rc.left;
	_height = rc.bottom - rc.top;

	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns>メッセージが存在する場合true</returns>
bool SimpleWindow::UpdateWindowMessage(void) {
	if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) {
		if (_msg.message == WM_QUIT) {
			_isQuitMessage = true;
		}
		else
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
		return true;
	}
	return false;
}