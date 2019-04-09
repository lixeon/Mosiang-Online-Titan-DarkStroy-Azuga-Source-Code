#include "../max_common/stdafx.h"

class CAnmExp;


void		SendTextToListbox(LPCTSTR msg,int id);
INT_PTR		CreatePanel(HINSTANCE hInst,HWND hWnd,CAnmExp* pExp);

extern HWND		g_hDlg;
