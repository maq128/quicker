#include "stdafx.h"

string _strWindowTitle;

BOOL CALLBACK MyEnumWindowsProc( HWND hwnd, LPARAM lParam )
{
    char buf[2048];
    int n = ::GetWindowText( hwnd, buf, sizeof(buf) );
    if ( n == _strWindowTitle.size() && _strWindowTitle.compare( buf ) == 0 ) {
        DWORD *pPID = (DWORD *)lParam;
        ::GetWindowThreadProcessId( hwnd, pPID );
    }
    return TRUE;
}

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main(int argc, _TCHAR* argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
/*
    SendBreak -pidfile <pidfile> | -pid <pid> | -title <title>
*/
    CCmdLine cmdLine;
    cmdLine.SplitLine( argc, argv );

    DWORD pid = 0;
    if ( cmdLine.GetArgumentCount( "-pid") == 1 ) {

        std::istringstream iss( cmdLine.GetArgument( "-pid", 0 ) );
        iss >> std::setbase(10) >> pid;

    } else if ( cmdLine.GetArgumentCount( "-pidfile") == 1 ) {

        string pidfile = cmdLine.GetArgument( "-pidfile", 0 );
        FILE *fp = ::fopen( pidfile.c_str(), "r" );
        if( fp != NULL ) {
            char buf[4096];
            if ( ::fgets( buf, sizeof(buf), fp ) != NULL ) {
                std::istringstream iss( buf );
                iss >> std::setbase(10) >> pid;
            }
            fclose(fp);
        }

    } else if ( cmdLine.GetArgumentCount( "-title") == 1 ) {

        _strWindowTitle = cmdLine.GetArgument( "-title", 0 );
        ::EnumWindows( MyEnumWindowsProc, (LPARAM)&pid );

    }

    if ( pid > 0 ) {
        ::AttachConsole( pid );
        ::SetConsoleCtrlHandler( NULL, TRUE );
        ::GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
        ::FreeConsole();
    }
    return 0;
}

