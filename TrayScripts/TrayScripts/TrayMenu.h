#pragma once
#include <Windows.h>
#include <unordered_map>
#include "ScriptParser.h"

class TrayMenu
{
public:
    void Init(HINSTANCE hInst, HWND handle);

    void AddTray();
    void ModifyTray();
    void RemoveTray();

    void CreateContextMenus(HWND hwnd);
    void ShowTrayMenu(HWND m_WindowHandle, int x, int y);

    void ActivateMenuItem(int wParam);
private:
    HICON LoadTrayIcon(HINSTANCE hInst, HWND handle);
    void ProcessTreeNode(XMLTree dataTree, HMENU currentMenuContext);

    NOTIFYICONDATA nid;
    HMENU m_HMenu;

    std::unordered_map<int, std::wstring> m_ScriptCommands;
};
