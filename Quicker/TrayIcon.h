// TrayIcon.h : interface of the CTrayIcon class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(_TrayIcon_H_)
#define _TrayIcon_H_

#pragma once

#include "shellapi.h"

const UINT UWM_NOTIFYICON = ::RegisterWindowMessage("NotifyIcon");

class CTrayIcon
{
protected:

    HWND m_hWnd;
    UINT m_uIconID;
    LPSTR m_lpToolTip;

public:

    CTrayIcon()
    {
        m_hWnd = NULL;
        m_uIconID = 0;
        m_lpToolTip = "";

        //Create window
        WNDCLASS wndclass;
        wndclass.style          = 0;
        wndclass.lpfnWndProc    = WindowProc;
        wndclass.cbClsExtra     = 0;
        wndclass.cbWndExtra     = 0;
        wndclass.hInstance      = ::GetModuleHandle(NULL);
        wndclass.hIcon          = 0;
        wndclass.hCursor        = 0;
        wndclass.hbrBackground  = 0;
        wndclass.lpszMenuName   = 0;
        wndclass.lpszClassName  = "CTrayIcon Helper Window";

        RegisterClass(&wndclass);

        m_hWnd = ::CreateWindow("CTrayIcon Helper Window", "CTrayIcon Helper Window", 0, 0, 0, 0, 0, 0, 0, 0, 0);
        ::SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
    }

    ~CTrayIcon()
    {
        //Destroy window
        if (m_hWnd) {
            ::DestroyWindow(m_hWnd);
            m_hWnd = NULL;
        }
    }

    //Processes event notifications sent by the system tray
    static LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
    {
        if (message == UWM_NOTIFYICON) {
            UINT uID = (UINT) wParam;
            UINT uMouseMsg = (UINT) lParam;
            CTrayIcon *pThis = (CTrayIcon *)::GetWindowLong(hWnd, GWL_USERDATA);
            if(uID == pThis->m_uIconID) {
                pThis->OnNotifyIcon(uMouseMsg);
            }
            return 0;
        }

        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }

    BOOL Add(UINT uIconID, HICON hIcon, LPSTR lpToolTip)
    {
        m_uIconID = uIconID;
        m_lpToolTip = lpToolTip;

        BOOL res;
        NOTIFYICONDATA tnid;

        tnid.cbSize = sizeof(NOTIFYICONDATA);
        tnid.hWnd = m_hWnd;
        tnid.uID = m_uIconID;
        tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        tnid.uCallbackMessage = UWM_NOTIFYICON;
        tnid.hIcon = hIcon;
        if (m_lpToolTip)
            lstrcpyn(tnid.szTip, m_lpToolTip, sizeof(tnid.szTip));
        else
            tnid.szTip[0] = '\0';

        res = Shell_NotifyIcon(NIM_ADD, &tnid);

        return res;
    }

    BOOL Delete()
    {
        BOOL res;
        NOTIFYICONDATA tnid;

        tnid.cbSize = sizeof(NOTIFYICONDATA);
        tnid.hWnd = m_hWnd;
        tnid.uID = m_uIconID;

        res = Shell_NotifyIcon(NIM_DELETE, &tnid);
        return res;
    }

    HWND GetHWND() {
        return m_hWnd;
    }

    virtual void OnNotifyIcon(UINT uMouseMsg)
    {
        OutputDebugString("OnNotifyIcon !\n");
    }
};

#endif // !defined(_TrayIcon_H_)
