// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlmisc.h>
#include <atlwin.h>
#include <atlddx.h>
#include <shlobj.h>

#include <vector>
typedef std::vector<CString> array;

#include "resource.h"

CString GetConfigFileName(const char *name);
void ReadFileLines(CString name, array& lines);
HMENU BuildPopupMenu(const array& config);
void ProcessMenuCommand(UINT cmd, const array& config);
void SplitStrings(CString str, TCHAR delim, array& tokens);
void JumpToRegedit(CString path);
