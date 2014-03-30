#include "stdafx.h"

#ifdef _DEBUG
void log( LPCTSTR fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	TCHAR buf[2048];
	_vstprintf( buf, fmt, args );
	::OutputDebugString( buf );
}
#else
#define log(...)
#endif

BOOL CALLBACK MyEnumWindowsProc( HWND hwnd, LPARAM lParam )
{
	CCmdLine *pCmdLine = (CCmdLine *)lParam;

	// 只处理符合 window class name 的窗口
	TCHAR buf[MAX_PATH];
	if ( ::GetClassName( hwnd, buf, MAX_PATH ) == 0 ) {
		return TRUE;
	}

	string sWindowClassName = pCmdLine->GetSafeArgument( "-wc", 0, "TXGuiFoundation" );
	if ( sWindowClassName.compare( buf ) != 0 ) {
		return TRUE;
	}

	// 只处理 WS_VISIBLE | WS_THICKFRAME 的窗口
	DWORD dwStyle = WS_VISIBLE | WS_THICKFRAME;
	if ( ( ::GetWindowLong( hwnd, GWL_STYLE ) & dwStyle ) != dwStyle ) {
		return TRUE;
	}

	// 取出原窗口位置
	RECT rc;
	if ( ! ::GetWindowRect( hwnd, &rc ) ) {
		return TRUE;
	}

	log("%08X rc: %d, %d / %d, %d\r\n", hwnd, rc.left, rc.top, rc.right, rc.bottom );

	if ( pCmdLine->GetArgumentCount( "-width" ) == 1 ) {
		int width = 0;
		std::istringstream iss( pCmdLine->GetArgument( "-width", 0 ) );
		iss >> std::setbase(10) >> width;
		int x = ( rc.left + rc.right ) / 2;
		rc.left = x - width / 2;
		rc.right = x + width / 2;
	} else if ( pCmdLine->GetArgumentCount( "-ex" ) == 1 ) {
		int ex = 0;
		std::istringstream iss( pCmdLine->GetArgument( "-ex", 0 ) );
		iss >> std::setbase(10) >> ex;
		rc.left = rc.left - ex;
		rc.right = rc.right + ex;
	}

	if ( pCmdLine->GetArgumentCount( "-height" ) == 1 ) {
		int height = 0;
		std::istringstream iss( pCmdLine->GetArgument( "-height", 0 ) );
		iss >> std::setbase(10) >> height;
		int y = ( rc.top + rc.bottom ) / 2;
		rc.top = y - height / 2;
		rc.bottom = y + height / 2;
	} else if ( pCmdLine->GetArgumentCount( "-ey" ) == 1 ) {
		int ey = 0;
		std::istringstream iss( pCmdLine->GetArgument( "-ey", 0 ) );
		iss >> std::setbase(10) >> ey;
		rc.top = rc.top - ey;
		rc.bottom = rc.bottom + ey;
	}

	log("         rc: %d, %d / %d, %d\r\n", rc.left, rc.top, rc.right, rc.bottom );

	if ( ! pCmdLine->HasSwitch( "-nolimit" ) ) {
        //int cx = ::GetSystemMetrics( SM_CXSCREEN );
        //int cy = ::GetSystemMetrics( SM_CYSCREEN );
        RECT rcDesktop;
        ::SystemParametersInfo( SPI_GETWORKAREA, 0, &rcDesktop, 0 );
        if ( rc.left < rcDesktop.left ) rc.left = rcDesktop.left;
        if ( rc.right > rcDesktop.right ) rc.right = rcDesktop.right;
        if ( rc.top < rcDesktop.top ) rc.top = rcDesktop.top;
        if ( rc.bottom > rcDesktop.bottom ) rc.bottom = rcDesktop.bottom;
	}

	// 如果调整结果超出范围，则不处理
	if ( rc.left >= rc.right || rc.top >= rc.bottom ) {
		return TRUE;
	}

	::SetWindowPos( hwnd, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER );
	return TRUE;
}

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main(int argc, _TCHAR* argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
/*
	WinRect [-wc <WindowClassName>] [-width <width> | -ex <width>] [-height <height> | -ey <height>] [-nolimit]
*/
	CCmdLine cmdLine;
	cmdLine.SplitLine( argc, argv );

	::EnumWindows( MyEnumWindowsProc, (LPARAM)&cmdLine );
	return 0;
}

