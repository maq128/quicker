#include "stdafx.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main(int argc, _TCHAR* argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
    HWND hWnd = ::GetForegroundWindow();
    ::ShowWindow( hWnd, SW_MINIMIZE );
	return 0;
}

