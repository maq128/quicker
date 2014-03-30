// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3CEEA6C5_91E0_4D50_B3F3_3F5C8F9C3F70__INCLUDED_)
#define AFX_STDAFX_H__3CEEA6C5_91E0_4D50_B3F3_3F5C8F9C3F70__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlmisc.h>
#include <atlddx.h>
#include <shlobj.h>

#include <vector>
typedef std::vector<CString> array;

#include "resource.h"

CString GetConfigFileName(const char *name);
void ReadFileLines(CString name, array& lines);
HMENU BuildPopupMenu( const array& config );
void ProcessMenuCommand( UINT cmd, const array& config );
void SplitStrings(CString str, TCHAR delim, array& tokens);
void JumpToRegedit(CString path);

#define MyTrace ATLTRACE("%s(%d) : ---- ", __FILE__, __LINE__); ATLTRACE

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3CEEA6C5_91E0_4D50_B3F3_3F5C8F9C3F70__INCLUDED_)
