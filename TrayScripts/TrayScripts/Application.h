#pragma once
#include <Windows.h>
#include "TrayMenu.h"

namespace WindowMessageCallbacks
{
    static	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static BOOL OnContextMenu(HWND handle, int x, int y);
};

class Application
{
    friend LRESULT CALLBACK WindowMessageCallbacks::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
    static Application*			GetInstance();

    int							Run();
    void						Terminate();

    void Initialise(HINSTANCE hInstance);
    void ShowContextMenu(int x, int y);

    void ActivateScript(int wParam);
private:
    Application();
    ~Application();

    static Application* s_Application;
    bool				m_Quit;

    void RegisterWindow(HINSTANCE hInstance);
    void CreateEmptyWindow();

    HWND m_WindowHandle;
    HINSTANCE m_HInstance;

    TrayMenu m_Tray;
};
