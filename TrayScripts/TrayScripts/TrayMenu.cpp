#include "TrayMenu.h"
#include <Windows.h>
#include <string>

#include "assert.h"
#include "UserMessages.h"

void TrayMenu::Init(HINSTANCE hInst, HWND handle)
{
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA)); //clear out the NOTIFYICONDATA class for use

    HICON icon = LoadTrayIcon(hInst, handle);

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = handle;
    nid.uID = 1;
    nid.uVersion = NOTIFYICON_VERSION_4;
    nid.hIcon = icon;
    nid.hBalloonIcon = icon;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.dwInfoFlags = NIIF_USER | NIIF_NOSOUND | NIF_MESSAGE;
    nid.uCallbackMessage = APPWM_ICONNOTIFY;
    std::wstring loadingMessage = L"Tray Scripts";
    wcscpy_s(nid.szInfoTitle, loadingMessage.c_str());
    wcscpy_s(nid.szInfo, loadingMessage.c_str());
    wcscpy_s(nid.szTip, L"Tray Icon");

    AddTray();
}

void TrayMenu::AddTray()
{
    Shell_NotifyIcon(NIM_ADD, &nid);
    Shell_NotifyIcon(NIM_SETVERSION, &nid);
    Shell_NotifyIcon(NIM_SETFOCUS, &nid);
}

void TrayMenu::ModifyTray()
{
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void TrayMenu::RemoveTray()
{
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

HICON TrayMenu::LoadTrayIcon(HINSTANCE hInst, HWND handle)
{
    HICON hIcon = static_cast<HICON>(LoadImage(hInst,
        TEXT("favicon.ico"),
        IMAGE_ICON,
        0, 0,
        LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE));

    SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    return hIcon;
}

//The purpose of this function is so we can recursively traverse down an xml tree adding submenu's and scripts as we need to
void TrayMenu::ProcessTreeNode(XMLTree dataTree, HMENU currentMenuContext)
{
#ifdef _DEBUG
    static std::vector<std::wstring> nodes;
#endif // _DEBUG

    XMLTree::const_iterator end = dataTree.end();
    //for (XMLTree::const_iterator it = dataTree.begin(); it != end; ++it)
    for (auto& it : dataTree)
    {
#ifdef _DEBUG
        nodes.push_back(it.first);
#endif // _DEBUG

        //add sub menu
        if (it.first == L"Menu")
        {
            HMENU hSubMenu = CreatePopupMenu();

            std::wstring const name = ScriptParser::GetAttribute(it.second, L"name");
            AppendMenuW(currentMenuContext, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, name.c_str());
            ProcessTreeNode(it.second, hSubMenu);
        }
        //add new script
        else if (it.first == L"Script")
        {
            static int uniqueScriptId(1);

            std::wstring const name = ScriptParser::GetAttribute(it.second, L"name");
            AppendMenuW(currentMenuContext, MF_STRING, ++uniqueScriptId, name.c_str());

            m_ScriptCommands[uniqueScriptId] = ScriptParser::GetAttribute(it.second, L"cmd");
        }
    }
}

//https://msdn.microsoft.com/en-us/library/windows/desktop/ms647558.aspx#_win32_Creating_Owner_Drawn_Menu_Items
void TrayMenu::CreateContextMenus(HWND hwnd)
{
    m_HMenu = CreatePopupMenu();

    XMLTree dataTree = ScriptParser::CreatePropertyTree("ScriptMenu.xml");

    //first is the root. We don't bother processing the root level
    auto rootNode = dataTree.begin()->second;
    ProcessTreeNode(rootNode, m_HMenu);

    AppendMenuW(m_HMenu, MF_STRING, 0, L"Exit");
    SetMenu(hwnd, m_HMenu);
}

void TrayMenu::ShowTrayMenu(HWND m_WindowHandle, int x, int y)
{
    assert(m_HMenu);

    TrackPopupMenu(m_HMenu,
        TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_VERNEGANIMATION,
        x,
        y,
        0,
        m_WindowHandle,
        NULL);
}

void TrayMenu::ActivateMenuItem(int wParam)
{
    //TODO - Consider changing to a more modern system of instantiating processes.
    std::wstring ws = m_ScriptCommands[wParam];
    std::string cmd = std::string(ws.begin(), ws.end());
    if (cmd.size())
    {
        system(cmd.c_str());
    }
}
