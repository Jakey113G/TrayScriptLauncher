#include <windows.h>
#include "Application.h"

int APIENTRY  WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    Application* app = Application::GetInstance();
    app->Initialise(hInstance);
    app->Run();
    ExitProcess(0);

    return 0;
}