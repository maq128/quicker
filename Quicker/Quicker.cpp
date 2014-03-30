#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "resource.h"

#include "aboutdlg.h"
#include "maindlg.h"

CAppModule _Module;

HWND _hCurrentWnd = NULL;

#include "TrayIcon.h"
#include "HKHandler.h"

class CMyTrayIcon : public CTrayIcon
{
public:
    CMyTrayIcon() : CTrayIcon() { }

    void Add(const char *tip)
    {
        if (m_uIconID != 0) {
            Delete();
        }
        HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
        BOOL bSucc = CTrayIcon::Add(1, hIcon, (LPSTR)tip);
        ::DestroyIcon(hIcon);
    }

    virtual void OnNotifyIcon(UINT uMouseMsg)
    {
        switch (uMouseMsg) {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        {
            array config;
            CString strConfigFile = GetConfigFileName( "config.ini" );
            ReadFileLines( strConfigFile, config );

            POINT pt;
            ::GetCursorPos(&pt);
            ::SetForegroundWindow(_hCurrentWnd);

            HMENU hPopupMenu = BuildPopupMenu( config );

            ::AppendMenu( hPopupMenu, MF_SEPARATOR, 0, NULL );
            ::AppendMenu( hPopupMenu, MF_STRING | MF_ENABLED, ID_CONFIG_EDIT, "编辑配置文件..." );
            ::AppendMenu( hPopupMenu, MF_SEPARATOR, 0, NULL );
            ::AppendMenu( hPopupMenu, MF_STRING | MF_ENABLED, ID_EXIT, "退出" );

            UINT cmd = ::TrackPopupMenuEx( hPopupMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON, pt.x, pt.y, _hCurrentWnd, NULL );
            ::DestroyMenu(hPopupMenu);

            if ( cmd == ID_EXIT ) {

                ::PostQuitMessage(0);

            } else if ( cmd == ID_CONFIG_EDIT ) {

                ::ShellExecute( NULL, NULL, (LPCTSTR)strConfigFile, NULL, NULL, SW_NORMAL );
                //::ShellExecute( NULL, NULL, "notepad.exe", (LPCTSTR)strConfigFile, NULL, SW_NORMAL );

            } else {

                ProcessMenuCommand( cmd, config );
            }
        }
            break;

        case WM_LBUTTONDBLCLK:
            break;
        }
    }
};

CMyTrayIcon _TrayIcon;
CHKManager _HKManager;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CString tip = "Quicker（快客）";
    _TrayIcon.Add((LPCTSTR)tip);
    _hCurrentWnd = _TrayIcon.GetHWND();

    _HKManager.RegisterAll();
    theLoop.AddMessageFilter(&_HKManager);

    int nRet = theLoop.Run();

    _HKManager.UnregisterAll();

    _TrayIcon.Delete();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//      HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
