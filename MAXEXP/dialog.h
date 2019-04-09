#pragma once


#include "../max_common/stdafx.h"

class CMaxExp;
interface I3DModel;


void		SendTextToListbox(LPCTSTR msg,int id);
INT_PTR		CreatePanel(HINSTANCE hInst,HWND hWnd,CMaxExp* pExp);
I3DModel*	GetRootModel();
DWORD		GetLightMapDetail();

extern HWND		g_hDlg;
