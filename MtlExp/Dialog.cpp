#include "mtlexp.h"
#include "../4DyuchiGRX_Common/IGeometry.h"

HWND g_hDlg = NULL;

I4DyuchiGXGeometry*		g_pGeometry = NULL;
I3DModel*				g_pRootModel = NULL;
I4DyuchiFileStorage*	g_pFileStorage = NULL;


I3DModel*	GetRootModel()
{
	return g_pRootModel;
}
void ReleaseModel()
{
	if (g_pRootModel)
	{
		g_pRootModel->Release();
		g_pRootModel = NULL;
	}
	if (g_pGeometry)
	{
		g_pGeometry->Release();
		g_pGeometry = NULL;
	}
	if (g_pFileStorage)
	{
		g_pFileStorage->Release();
		g_pFileStorage = NULL;
	}
}

BOOL CALLBACK DlgProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam);


INT_PTR CreatePanel(HINSTANCE hInst,HWND hWnd)
{
	
	return DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_EXPORT),hWnd,DlgProc,0);
	
}
BOOL OpenFileDlg(char* szFileName,DWORD dwSize,HWND hWnd, char* szFilter)
{
    OPENFILENAME ofn;
    char szDirName[_MAX_DIR];
	char szFileTitle[] = "Load .Mod File";

	GetCurrentDirectory(_MAX_DIR,szDirName);

    szFileName[0] = '\0';

	char	filter[256];
	memset(filter,0,sizeof(filter));
	DWORD	dwLen = lstrlen(szFilter);
	memcpy(filter,szFilter,dwLen);

    for (DWORD i=0; i<dwLen; i++) 
	{		
		if (filter[i] == '|')
			filter[i] = '\0';
	}

    memset(&ofn, 0, sizeof(OPENFILENAME));

    ofn.lStructSize     = sizeof(OPENFILENAME);
    ofn.hwndOwner       = hWnd;
    ofn.lpstrFilter     = filter;
    ofn.nFilterIndex    = 1;
    ofn.lpstrFile       = szFileName;
    ofn.nMaxFile        = dwSize;
    ofn.lpstrFileTitle  = szFileTitle;
    ofn.nMaxFileTitle   = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    BOOL bResult = GetOpenFileName(&ofn);
	if (bResult)
		lstrcpy(szFileName,ofn.lpstrFile);
	
	return bResult;
}

BOOL CALLBACK DlgProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{

	case WM_INITDIALOG:
		{
			g_hDlg = hdlg;
		}
		break;
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_BUTTON_ROOT_MODEL:
			{

				char	szFileName[_MAX_PATH];
				memset(szFileName,0,sizeof(szFileName));
				if (OpenFileDlg(szFileName,_MAX_PATH-1,g_hDlg, "MAX Custom Binary Exporter(*.MOD)|*.MOD|All Files(*.*)|*.*|"))
				{
				
					if (!g_pGeometry)
					{

						HRESULT	hr;
						hr = CoCreateInstance(
							CLSID_4DyuchiGXGeometry,
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_4DyuchiGXGeometry,
							(void**)&g_pGeometry);

						if (S_OK != hr)
						{
							MessageBox(NULL,"Fail to Load I4DyuchiGXGeometry!","Error",MB_OK);
							return FALSE;
						}
						
						hr = CoCreateInstance(
							CLSID_4DyuchiFileStorage,
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_4DyuchiFileStorage,
							(void**)&g_pFileStorage);

						if (hr != S_OK)
						{
							MessageBox(NULL,"Fail to Load I4DyuchiFileStorage!","Error",MB_OK);
							return FALSE;
						}
						g_pFileStorage->Initialize(0,1024,_MAX_PATH,FILE_ACCESS_METHOD_ONLY_FILE);
						g_pGeometry->Initialize(NULL,g_pFileStorage,32,1000,100,NULL);	// 초기화

					}

					if (g_pRootModel)
					{
						g_pRootModel->Release();
						g_pRootModel = NULL;
					}

					if (0xffffffff == g_pGeometry->LoadModel(&g_pRootModel,szFileName,0))
					{
						MessageBox(NULL,"Fail to Load Model!","Error",MB_OK);
						return FALSE;
					}

					SetDlgItemText(g_hDlg,IDC_EDIT_ROOT_MODEL_FILE,szFileName);
				}
			}
			break;
			case IDC_BUTTON_EXPORT:
			{
				if (!g_pRootModel)
				{
					MessageBox(NULL,"모델이 바르게 지정되어있지 않습니다","Error",MB_OK);
				}
				else
				{
					EndDialog(g_hDlg,1);
				}
			}

			break;

		}
		break;
	case WM_CLOSE:
		EndDialog(g_hDlg,0);
		break;

	}
	return FALSE;
}
