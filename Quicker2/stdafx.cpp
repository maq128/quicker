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
		// ����޷��ԡ������ķ�ʽ�򿪣�������ļ������ڣ���������ͼ����һ���µ�
		fp = ::fopen((LPCTSTR)name, "w");
		if (fp != NULL) {
			::fputs("# ���ָ���� EXPLORER_GROUP���˺���ļ������ʾ��������˵��¡�\n", fp);
			::fputs("# ����д�������������� Quicker ������Ч��\n", fp);
			::fputs("\n", fp);
			::fputs("# EXPLORER_GROUP=�ļ���\n", fp);
			::fputs("\n", fp);
			::fputs("# EXPLORER=<�ļ���ȫ·��>\n", fp);
			::fputs("# ����д�������������� Quicker ������Ч��\n", fp);
			::fputs("\n", fp);
			::fputs("EXPLORER=C:\\\n", fp);
			::fputs("\n", fp);
			::fputs("# ���ָ���� REGEDIT_GROUP���˺��ע������ʾ��������˵��¡�\n", fp);
			::fputs("# ����д�������������� Quicker ������Ч��\n", fp);
			::fputs("\n", fp);
			::fputs("REGEDIT_GROUP=ע���\n", fp);
			::fputs("\n", fp);
			::fputs("# REGEDIT=<ע���ȫ·��>\n", fp);
			::fputs("# ����д�������������� Quicker ������Ч��\n", fp);
			::fputs("\n", fp);
			::fputs("REGEDIT=HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\n", fp);
			::fputs("\n", fp);
			::fputs("# ���ָ���� CMD_GROUP���˺�����������ʾ��������˵��¡�\n", fp);
			::fputs("# ����д�������������� Quicker ������Ч��\n", fp);
			::fputs("\n", fp);
			::fputs("CMD_GROUP=���ó���\n", fp);
			::fputs("\n", fp);
			::fputs("# CMD=<˵������>|<������>\n", fp);
			::fputs("# ����д�������������� Quicker ������Ч��\n", fp);
			::fputs("\n", fp);
			::fputs("CMD=���±�|notepad.exe\n", fp);
			::fputs("\n", fp);
			::fputs("# HOTKEY=<�ȼ�����>|<������>\n", fp);
			::fputs("# ����д���������� Quicker ����Ч��\n", fp);
			::fputs("# ���ȼ����������üӺŷָ��Ķ����ɣ�ÿ�ε���Ч����Ϊ��\n", fp);
			::fputs("#       MOD_ALT\n", fp);
			::fputs("#       MOD_CONTROL\n", fp);
			::fputs("#       MOD_SHIFT\n", fp);
			::fputs("#       MOD_WIN\n", fp);
			::fputs("#       һ����д�ַ���A��Z��\n", fp);
			::fputs("#       һ�����֣�0��9��\n", fp);
			::fputs("# �������С�Ϊһ���������������ַ������ɴ�����������������·��������� Quicker.exe��\"QuickerPopupMenu\" Ϊ���Ᵽ���֡�\n", fp);
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
	// ���� RegEdit
	::ShellExecute(NULL, NULL, "regedit.exe", NULL, NULL, SW_NORMAL);

	// �ҵ� RegEdit ��������
	HWND hwndMain = ::FindWindow("RegEdit_RegEdit", NULL);
	for (int t = 500; hwndMain == NULL && t <= 2500; t += 1000) {
		::Sleep(t);
		hwndMain = ::FindWindow("RegEdit_RegEdit", NULL);
	}
	if (hwndMain == NULL) return;

	// �ҵ� RegEdit �е� TreeCtrl
	HWND hwndTree = ::FindWindowEx(hwndMain, NULL, "SysTreeView32", NULL);
	if (hwndTree == NULL) return;

	// ���� TreeCtrl �Ĵ��ڲ���� process id
	DWORD pid = 0L;
	::GetWindowThreadProcessId(hwndTree, &pid);

	// �õ�һ��������Ӧ����Ȩ�޵� process handle
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
	if (hProcess == NULL) return;

	// ��Ŀ������з���һ���ڴ�
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
			// �ڱ������й��� TVITEMEX ���ݿ飬�����Ƶ�Ŀ�������
			TVITEMEX ti;
			ti.mask = TVIF_TEXT;
			ti.hItem = hChild;
			ti.pszText = (LPSTR)(pBlock)+sizeof(TVITEMEX);
			ti.cchTextMax = MAX_PATH;
			::WriteProcessMemory(hProcess, pBlock, &ti, sizeof(ti), NULL);

			// �� TreeCtrl ������Ϣ����ȡ�� item ��Ϣ
			TreeView_GetItem(hwndTree, pBlock);

			// ��Ŀ������а� TVITEMEX ���ݿ鸴�ƻ���
			::ReadProcessMemory(hProcess, pBlock, &ti, sizeof(TVITEMEX), NULL);

			// ��Ŀ������а� label text ���ƻ���
			CHAR buf[MAX_PATH];
			::ReadProcessMemory(hProcess, ti.pszText, buf, ti.cchTextMax, NULL);

			// ����ҵ�ָ����ע�������ѭ����������������
			if (branch.Compare(buf) == 0) {
				TreeView_SelectItem(hwndTree, hChild);
				TreeView_Expand(hwndTree, hChild, TVE_EXPAND);
				break;
			}

			hChild = TreeView_GetNextItem(hwndTree, hChild, TVGN_NEXT);
		}
		if (hChild == NULL) {
			// û��ƥ��� branch
			path = "";
			break;
		}
		hParent = hChild;
	}

	// ��Ŀ��������ͷ��ڴ��
	::VirtualFreeEx(hProcess, pBlock, nBlock, MEM_DECOMMIT);
	::VirtualFreeEx(hProcess, pBlock, 0, MEM_RELEASE);

	// �ر� process handle
	::CloseHandle(hProcess);
}
