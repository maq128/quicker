#include "stdafx.h"

#include "HKHandler.h"

extern HWND _hCurrentWnd;

int CHKHandlerBase::seq = 0;

CHKHandlerBase::CHKHandlerBase()
{
    m_hkMod = 0;
    m_hkVK = 0;
    CString str;
    str.Format( "HotKeyHandler_%d", seq++ );
    m_hkID = ::GlobalAddAtom( (LPCTSTR)str );
}

BOOL CHKHandlerBase::parseHotKeySpec( LPCTSTR spec )
{
    array tokens;
    SplitStrings( spec, '+', tokens );
    for ( array::iterator it = tokens.begin(); it != tokens.end(); it++ ) {
        CString token = (*it);
        if ( token.CompareNoCase( "MOD_ALT" ) == 0 ) {
            m_hkMod |= MOD_ALT;
        } else if ( token.CompareNoCase( "MOD_CONTROL" ) == 0 ) {
            m_hkMod |= MOD_CONTROL;
        } else if ( token.CompareNoCase( "MOD_SHIFT" ) == 0 ) {
            m_hkMod |= MOD_SHIFT;
        } else if ( token.CompareNoCase( "MOD_WIN" ) == 0 ) {
            m_hkMod |= MOD_WIN;
        } else {
            int code = token.GetAt( 0 );
            if ( ( code >= '0' && code <= '9' ) || ( code >= 'A' && code <= 'Z' ) ) {
                m_hkVK = code;
            }
        }
    }
    return m_hkMod != 0 && m_hkVK != 0;
}

// 弹出菜单
class CHandlerPopupMenu : public CHKHandlerBase
{
public:
    void Process() {
        array config;
        CString strConfigFile = GetConfigFileName( "config.ini" );
        ReadFileLines( strConfigFile, config );

        POINT pt;
        ::GetCursorPos(&pt);
        ::SetForegroundWindow(_hCurrentWnd);

        HMENU hPopupMenu = BuildPopupMenu( config );
        ::AppendMenu( hPopupMenu, MF_SEPARATOR, 0, NULL );
        ::AppendMenu( hPopupMenu, MF_STRING | MF_ENABLED, ID_CONFIG_EDIT, "编辑配置文件..." );

        UINT cmd = ::TrackPopupMenuEx( hPopupMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON, pt.x, pt.y, _hCurrentWnd, NULL );
        ::DestroyMenu(hPopupMenu);

        if ( cmd == ID_CONFIG_EDIT ) {

            ::ShellExecute( NULL, NULL, (LPCTSTR)strConfigFile, NULL, NULL, SW_NORMAL );
            //::ShellExecute( NULL, NULL, "notepad.exe", (LPCTSTR)strConfigFile, NULL, SW_NORMAL );

        } else {

            ProcessMenuCommand( cmd, config );
        }
    }
};

// 直接执行外部命令
class CHandlerExternal : public CHKHandlerBase
{
    CString m_strCmdLine;
public:
    CHandlerExternal( LPCTSTR cmd ) {
        m_strCmdLine = cmd;
    }

    void Process() {
        STARTUPINFO si;
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi;
        memset(&pi, 0, sizeof(pi));

        BOOL cp = ::CreateProcess(
            NULL,
            (LPTSTR)(LPCTSTR)m_strCmdLine,
            NULL, NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si, &pi);
    }
};

void CHKManager::RegisterAll()
{
    array  config;
    CString strConfigFile = GetConfigFileName( "config.ini" );
    ReadFileLines( strConfigFile, config );

    for ( array::iterator it = config.begin(); it != config.end(); it++ ) {
        CString line = (*it);
        CString entry = line.Left( 7 );
        if ( entry.CompareNoCase( "HOTKEY=" ) != 0 ) continue;

        line = line.Mid( 7 );
        int pos = line.Find( '|' );
        if ( pos < 0 ) continue;

        CString spec = line.Left( pos );    spec.TrimLeft(); spec.TrimRight();
        CString cmd = line.Mid( pos + 1 );  cmd.TrimLeft();  cmd.TrimRight();
        if ( spec.IsEmpty() || cmd.IsEmpty() ) continue;

        if ( cmd.CompareNoCase( "QuickerPopupMenu" ) == 0 ) {
            CHKHandlerBase *handler = new CHandlerPopupMenu();
            if ( handler->parseHotKeySpec( spec ) ) {
                m_handlerList.Add( handler );
            } else {
                delete handler;
            }
        } else {
            CHKHandlerBase *handler = new CHandlerExternal( cmd );
            if ( handler->parseHotKeySpec( spec ) ) {
                m_handlerList.Add( handler );
            } else {
                delete handler;
            }
        }
    }

    for (int i=0; i<m_handlerList.GetSize(); i++) {
        BOOL b = ::RegisterHotKey(NULL, m_handlerList[i]->m_hkID, m_handlerList[i]->m_hkMod, m_handlerList[i]->m_hkVK);
		if (!b) {
			DWORD err = ::GetLastError();
			if (err != 0x00000581) {
				break;
			}
		}
	}
}

void CHKManager::UnregisterAll()
{
    for (int i=0; i<m_handlerList.GetSize(); i++) {
        ::UnregisterHotKey(NULL, m_handlerList[i]->m_hkID);
        ::GlobalDeleteAtom(m_handlerList[i]->m_hkID);
        delete m_handlerList[i];
    }
}

BOOL CHKManager::PreTranslateMessage(MSG* pMsg)
{
    BOOL bProcessed = FALSE;

    if (pMsg->message == WM_HOTKEY) {
//        MyTrace("WM_HOTKEY - wParam(%08X) lParam(%08X)\r\n", pMsg->wParam, pMsg->lParam);
        for (int i=0; i<m_handlerList.GetSize(); i++) {
            if (pMsg->wParam == m_handlerList[i]->m_hkID) {
                m_handlerList[i]->Process();
                bProcessed = TRUE;
                break;
            }
        }
    }

    return bProcessed;
}
