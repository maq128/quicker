// stdafx.cpp : source file that includes just the standard includes
//	Quicker2.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

CString GetConfigFileName(const char *name)
{
	CString strFilename = "C:\\";
	strFilename += name;

	char buf[MAX_PATH];
	BOOL b = ::SHGetSpecialFolderPath(NULL, buf, CSIDL_APPDATA, FALSE);
	if (b) {
		strFilename = buf;
		strFilename += "\\Quicker";
		::CreateDirectory((LPCTSTR)strFilename, NULL);
		strFilename += "\\";
		strFilename += name;
	}

	return strFilename;
}

void ReadFileLines(CString name, array& lines)
{

	int idx = 0;
	FILE *fp = ::fopen((LPCTSTR)name, "r");
	if (fp == NULL) {
		// 如果无法以“读”的方式打开，则可能文件不存在，所以先试图创建一个新的
		fp = ::fopen((LPCTSTR)name, "w");
		if (fp != NULL) {
			::fputs("# 如果指定了 EXPLORER_GROUP，此后的文件夹项将显示在其二级菜单下。\n", fp);
			::fputs("# 可以写多条，不用重启 Quicker 即可生效。\n", fp);
			::fputs("\n", fp);
			::fputs("# EXPLORER_GROUP=文件夹\n", fp);
			::fputs("\n", fp);
			::fputs("# EXPLORER=<文件夹全路径>\n", fp);
			::fputs("# 可以写多条，不用重启 Quicker 即可生效。\n", fp);
			::fputs("\n", fp);
			::fputs("EXPLORER=C:\\\n", fp);
			::fputs("\n", fp);
			::fputs("# 如果指定了 REGEDIT_GROUP，此后的注册表项将显示在其二级菜单下。\n", fp);
			::fputs("# 可以写多条，不用重启 Quicker 即可生效。\n", fp);
			::fputs("\n", fp);
			::fputs("REGEDIT_GROUP=注册表\n", fp);
			::fputs("\n", fp);
			::fputs("# REGEDIT=<注册表全路径>\n", fp);
			::fputs("# 可以写多条，不用重启 Quicker 即可生效。\n", fp);
			::fputs("\n", fp);
			::fputs("REGEDIT=HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\n", fp);
			::fputs("\n", fp);
			::fputs("# 如果指定了 CMD_GROUP，此后的命令行项将显示在其二级菜单下。\n", fp);
			::fputs("# 可以写多条，不用重启 Quicker 即可生效。\n", fp);
			::fputs("\n", fp);
			::fputs("CMD_GROUP=常用程序\n", fp);
			::fputs("\n", fp);
			::fputs("# CMD=<说明文字>|<命令行>\n", fp);
			::fputs("# 可以写多条，不用重启 Quicker 即可生效。\n", fp);
			::fputs("\n", fp);
			::fputs("CMD=记事本|notepad.exe\n", fp);
			::fputs("\n", fp);
			::fputs("# HOTKEY=<热键描述>|<命令行>\n", fp);
			::fputs("# 可以写多条，重启 Quicker 后生效。\n", fp);
			::fputs("# “热键描述”由用加号分隔的多段组成，每段的有效内容为：\n", fp);
			::fputs("#       MOD_ALT\n", fp);
			::fputs("#       MOD_CONTROL\n", fp);
			::fputs("#       MOD_SHIFT\n", fp);
			::fputs("#       MOD_WIN\n", fp);
			::fputs("#       一个大写字符（A～Z）\n", fp);
			::fputs("#       一个数字（0～9）\n", fp);
			::fputs("# “命令行”为一个完整的命令行字符串（可带参数），如果是相对路径则相对于 Quicker.exe，\"QuickerPopupMenu\" 为特殊保留字。\n", fp);
			::fputs("\n", fp);
			::fputs("HOTKEY=MOD_WIN+S|QuickerPopupMenu\n", fp);
			::fputs("HOTKEY=MOD_WIN+A|WinMin.exe\n", fp);
			::fputs("HOTKEY=MOD_WIN+Z|WinRect.exe -wc TXGuiFoundation -width 900 -height 600\n", fp);
			::fputs("HOTKEY=MOD_WIN+Q|Speaker.exe\n", fp);
			::fputs("HOTKEY=MOD_WIN+P|Power.exe -suspend\n", fp);
			::fputs("HOTKEY=MOD_WIN+H|Power.exe -hibernate\n", fp);
		}
		::fclose(fp);
		fp = ::fopen((LPCTSTR)name, "r");
	}

	if (fp != NULL) {
		char buf[4096];
		while (fgets(buf, sizeof(buf), fp) != NULL) {
			CString strPath = buf;

			strPath.TrimLeft(" \r\n\t");
			strPath.TrimRight(" \r\n\t");
			if (strPath.GetLength() == 0) continue;
			if (strPath.GetAt(0) == '#') continue;

			lines.push_back(strPath);
		}
		fclose(fp);
	}
}

HMENU BuildPopupMenu(const array& config)
{
	//HBITMAP hBitmapFolder = NULL;
	//hBitmapFolder = ::LoadBitmap( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_FOLDER) );
	HMENU hPopupMenu = ::CreatePopupMenu();
	HMENU hPathGroup = NULL;
	HMENU hRegGroup = NULL;
	HMENU hCmdGroup = NULL;

	for (int i = 0; i < config.size(); i++) {
		CString line = config[i];

		if (line.Left(15).CompareNoCase("EXPLORER_GROUP=") == 0) {

			CString group = line.Mid(15);
			hPathGroup = ::CreatePopupMenu();
			::AppendMenu(hPopupMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPathGroup, (LPCTSTR)group);

		}
		else if (line.Left(9).CompareNoCase("EXPLORER=") == 0) {

			CString path = line.Mid(9);
			if (hPathGroup == NULL) {
				::AppendMenu(hPopupMenu, MF_STRING | MF_ENABLED, ID_QUICKSTART_FIRST + i, (LPCTSTR)path);
			}
			else {
				::AppendMenu(hPathGroup, MF_STRING | MF_ENABLED, ID_QUICKSTART_FIRST + i, (LPCTSTR)path);
			}

		}
		else if (line.Left(14).CompareNoCase("REGEDIT_GROUP=") == 0) {

			CString group = line.Mid(14);
			hRegGroup = ::CreatePopupMenu();
			::AppendMenu(hPopupMenu, MF_STRING | MF_POPUP, (UINT_PTR)hRegGroup, (LPCTSTR)group);

		}
		else if (line.Left(8).CompareNoCase("REGEDIT=") == 0) {

			CString path = line.Mid(8);
			if (hRegGroup == NULL) {
				::AppendMenu(hPopupMenu, MF_STRING | MF_ENABLED, ID_QUICKSTART_FIRST + i, (LPCTSTR)path);
			}
			else {
				::AppendMenu(hRegGroup, MF_STRING | MF_ENABLED, ID_QUICKSTART_FIRST + i, (LPCTSTR)path);
			}

		}
		else if (line.Left(10).CompareNoCase("CMD_GROUP=") == 0) {

			CString group = line.Mid(10);
			hCmdGroup = ::CreatePopupMenu();
			::AppendMenu(hPopupMenu, MF_STRING | MF_POPUP, (UINT_PTR)hCmdGroup, (LPCTSTR)group);

		}
		else if (line.Left(4).CompareNoCase("CMD=") == 0) {

			CString cmdline = line.Mid(4);
			CString title = cmdline;
			int pos = cmdline.Find('|');
			if (pos > 0) {
				title = cmdline.Left(pos);
				cmdline = cmdline.Mid(pos + 1);
			}
			if (hCmdGroup == NULL) {
				::AppendMenu(hPopupMenu, MF_STRING | MF_ENABLED, ID_QUICKSTART_FIRST + i, (LPCTSTR)title);
			}
			else {
				::AppendMenu(hCmdGroup, MF_STRING | MF_ENABLED, ID_QUICKSTART_FIRST + i, (LPCTSTR)title);
			}

		}
	}

	return hPopupMenu;
}

void ProcessMenuCommand(UINT cmd, const array& config)
{
	if (cmd >= ID_QUICKSTART_FIRST && cmd <= ID_QUICKSTART_LAST) {
		CString line = config[cmd - ID_QUICKSTART_FIRST];

		if (line.Left(9).CompareNoCase("EXPLORER=") == 0) {

			CString path = "/e, ";
			path += line.Mid(9);
			::ShellExecute(NULL, NULL, "explorer.exe", (LPCTSTR)path, NULL, SW_NORMAL);

		}
		else if (line.Left(8).CompareNoCase("REGEDIT=") == 0) {

			CString path = line.Mid(8);
			JumpToRegedit(path);

		}
		else if (line.Left(4).CompareNoCase("CMD=") == 0) {

			CString cmdline = line.Mid(4);
			CString title = cmdline;
			int pos = cmdline.Find('|');
			if (pos > 0) {
				title = cmdline.Left(pos);
				cmdline = cmdline.Mid(pos + 1);
			}

			STARTUPINFO si;
			memset(&si, 0, sizeof(si));
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(pi));

			BOOL cp = ::CreateProcess(
				NULL,
				(LPTSTR)(LPCTSTR)cmdline,
				NULL, NULL,
				FALSE,
				CREATE_NEW_CONSOLE,
				NULL,
				NULL,
				&si, &pi);
		}
	}
}

void SplitStrings(CString str, TCHAR delim, array& tokens)
{
	int pos0 = 0;
	int pos1 = str.Find(delim, pos0);
	while (pos1 > pos0) {
		CString token = str.Mid(pos0, (pos1 - pos0));
		token.TrimLeft();
		token.TrimRight();
		if (!token.IsEmpty()) {
			tokens.push_back(token);
		}
		pos0 = pos1 + 1;
		pos1 = str.Find(delim, pos0);
	}
	CString token = str.Mid(pos0);
	token.TrimLeft();
	token.TrimRight();
	if (!token.IsEmpty()) {
		tokens.push_back(token);
	}
}

void JumpToRegedit(CString path)
{
	// 运行 RegEdit
	::ShellExecute(NULL, NULL, "regedit.exe", NULL, NULL, SW_NORMAL);

	// 找到 RegEdit 的主窗口
	HWND hwndMain = ::FindWindow("RegEdit_RegEdit", NULL);
	for (int t = 500; hwndMain == NULL && t <= 2500; t += 1000) {
		::Sleep(t);
		hwndMain = ::FindWindow("RegEdit_RegEdit", NULL);
	}
	if (hwndMain == NULL) return;

	// 找到 RegEdit 中的 TreeCtrl
	HWND hwndTree = ::FindWindowEx(hwndMain, NULL, "SysTreeView32", NULL);
	if (hwndTree == NULL) return;

	// 根据 TreeCtrl 的窗口查出其 process id
	DWORD pid = 0L;
	::GetWindowThreadProcessId(hwndTree, &pid);

	// 得到一个具有相应访问权限的 process handle
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
	if (hProcess == NULL) return;

	// 在目标进程中分配一块内存
	int nBlock = sizeof(TVITEMEX)+sizeof(MAX_PATH);
	LPVOID pBlock = ::VirtualAllocEx(hProcess, NULL, nBlock, MEM_COMMIT, PAGE_READWRITE);

	HTREEITEM hParent = TreeView_GetRoot(hwndTree);
	while (!path.IsEmpty()) {
		CString branch;
		int pos = path.Find('\\');
		if (pos > 0) {
			branch = path.Left(pos);
			path = path.Mid(pos + 1);
		}
		else {
			branch = path;
			path = "";
		}

		HTREEITEM hChild = TreeView_GetNextItem(hwndTree, hParent, TVGN_CHILD);
		while (hChild != NULL) {
			// 在本进程中构建 TVITEMEX 数据块，并复制到目标进程中
			TVITEMEX ti;
			ti.mask = TVIF_TEXT;
			ti.hItem = hChild;
			ti.pszText = (LPSTR)(pBlock)+sizeof(TVITEMEX);
			ti.cchTextMax = MAX_PATH;
			::WriteProcessMemory(hProcess, pBlock, &ti, sizeof(ti), NULL);

			// 向 TreeCtrl 发送消息，获取其 item 信息
			TreeView_GetItem(hwndTree, pBlock);

			// 从目标进程中把 TVITEMEX 数据块复制回来
			::ReadProcessMemory(hProcess, pBlock, &ti, sizeof(TVITEMEX), NULL);

			// 从目标进程中把 label text 复制回来
			CHAR buf[MAX_PATH];
			::ReadProcessMemory(hProcess, ti.pszText, buf, ti.cchTextMax, NULL);

			// 如果找到指定的注册项，跳出循环，继续向下游找
			if (branch.Compare(buf) == 0) {
				TreeView_SelectItem(hwndTree, hChild);
				TreeView_Expand(hwndTree, hChild, TVE_EXPAND);
				break;
			}

			hChild = TreeView_GetNextItem(hwndTree, hChild, TVGN_NEXT);
		}
		if (hChild == NULL) {
			// 没有匹配的 branch
			path = "";
			break;
		}
		hParent = hChild;
	}

	// 在目标进程中释放内存块
	::VirtualFreeEx(hProcess, pBlock, nBlock, MEM_DECOMMIT);
	::VirtualFreeEx(hProcess, pBlock, 0, MEM_RELEASE);

	// 关闭 process handle
	::CloseHandle(hProcess);
}
