#include "Application.h"
#include <Windows.h>
#include <windowsx.h> //for defines for x param
#include <string>

#include "assert.h"
#include "UserMessages.h"


LRESULT CALLBACK WindowMessageCallbacks::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_SIZE:
        break;
    case WM_CLOSE:
    case WM_DESTROY:
    {
        Application::s_Application->Terminate();
        break;
    }
    case WM_CONTEXTMENU:
    {
        break;
    }
    //This is the callback function when the notification tray icon is interacted with
    case APPWM_ICONNOTIFY:
    {
        //use loword because in the high, we store the uId of the notification icon - which is 1
        switch (LOWORD(lParam))
        {
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_CONTEXTMENU:
            {
                Application::GetInstance()->ShowContextMenu(GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam));
                break;
            }
        }
        break;
    }
    //Process the selected item in the context menu
    case WM_COMMAND:
    {
        switch (wParam)
        {
            case NID_ExitTrayCommand:
            {
                Application::s_Application->Terminate();
                return 0;
            }
            default:
            {
                Application::GetInstance()->ActivateScript(static_cast<int>(wParam));
                break;
            }
        }
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

//Helpful function if we needed to convert mouse coords to screen, to see if we are in a particular space
BOOL WindowMessageCallbacks::OnContextMenu(HWND handle, int x, int y)
{
    RECT rc;                    // client area of window 
    POINT pt = { x, y };        // location of mouse click 

    // Get the bounding rectangle of the client area. 
    GetClientRect(handle, &rc);

    // Convert the mouse position to client coordinates. 
    ScreenToClient(handle, &pt);

    // If the position is in the client area, display a shortcut menu. 
    if (PtInRect(&rc, pt))
    {
        ClientToScreen(handle, &pt);
        return TRUE;
    }

    // Return FALSE if no menu is displayed. 
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Application* Application::s_Application;

void Application::RegisterWindow(HINSTANCE hInstance)
{
    WNDCLASSEXW wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEXW)); //clear out the window class for use
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowMessageCallbacks::WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"Tray_Scripts";

    RegisterClassExW(&wc);
}

void Application::CreateEmptyWindow()
{
    //Uses the registered window via the lpszClassName name to make a new window
    m_WindowHandle = CreateWindowExW(NULL,
        L"Tray_Scripts",
        L"Tray Scripts",
        WS_ICONIC,
        0,
        0,
        0,
        0,
        NULL, //Parent window
        NULL, //menus - we sort out menu later
        m_HInstance, //application handle 
        NULL); //multiple window encapsulation

    assert(m_WindowHandle);
}

Application* Application::GetInstance()
{
    if (s_Application == nullptr)
    {
        s_Application = new Application();
    }
    return s_Application;
}

int Application::Run()
{
    MSG msg;

    while (!m_Quit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //peek for windows message
            if (msg.message == WM_QUIT)
            {
                Terminate();
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    return 0;
}

void Application::Terminate()
{
    m_Quit = true;
    m_Tray.RemoveTray();
}

void Application::Initialise(HINSTANCE hInstance)
{
    m_HInstance = hInstance;
    RegisterWindow(hInstance);
    CreateEmptyWindow();

    m_Tray.Init(hInstance, m_WindowHandle);
    m_Tray.CreateContextMenus(m_WindowHandle);
}

void Application::ShowContextMenu(int x, int y)
{
    SetForegroundWindow(m_WindowHandle);
    m_Tray.ShowTrayMenu(m_WindowHandle, x, y);
}

void Application::ActivateScript(int wParam)
{
    m_Tray.ActivateMenuItem(wParam);
}

Application::Application() : m_Quit(false)
{
}

Application::~Application()
{
}
