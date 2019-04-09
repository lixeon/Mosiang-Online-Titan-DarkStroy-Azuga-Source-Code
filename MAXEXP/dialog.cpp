#include "dialog.h"
#include "resource.h"
#include "export.h"
#include "object_property.h"
#include "../4DyuchiGRX_Common/IGeometry.h"


CObjectPropertyList g_propertyList;

HWND g_hDlg = NULL;
CMaxExp* g_pExp = NULL;

I4DyuchiGXGeometry*		g_pGeometry = NULL;
I4DyuchiFileStorage*	g_pFileStorage = NULL;
I3DModel*				g_pRootModel = NULL;

I3DModel*	GetRootModel()
{
	return g_pRootModel;
}
BOOL CALLBACK DlgProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam);

DWORD GetLightMapDetail()
{
	DWORD	dwDetailConst = 1;
	if (BST_CHECKED == SendDlgItemMessage(g_hDlg,IDC_RADIO_LIGHTMAP_DETAIL_X4,BM_GETCHECK,0,0))
	{
		dwDetailConst = 4;
	}
	else if (BST_CHECKED == SendDlgItemMessage(g_hDlg,IDC_RADIO_LIGHTMAP_DETAIL_X2,BM_GETCHECK,0,0))
	{
		dwDetailConst = 2;
	}
	else
	{
		dwDetailConst = 1;
	}
	return dwDetailConst;

}
void SendTextToListbox(LPCTSTR msg,int id)
{
	SendDlgItemMessage(g_hDlg,id, LB_ADDSTRING, 0,(DWORD)msg);         

}
INT_PTR		CreatePanel(HINSTANCE hInst,HWND hWnd,CMaxExp* pExp)
{
	g_pExp = pExp;
	return DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_EXPORT),hWnd,DlgProc,(LPARAM)pExp);
}
/*
BOOL OpenFileName(char* szFileName)
{
	
	OPENFILENAME ofn;
    
	char szDirName[_MAX_DIR], szFile[256], szFileTitle[256];
    int i;

    _getcwd(szDirName, _MAX_DIR); // 현재 작업 디렉토리를 가진다 
    szFile[0] = '\0';

    for (i = 0; szFilter[i] != '\0'; i++) 
        if (szFilter[i] == '|')
            szFilter[i] = '\0';

    memset(&ofn, 0, sizeof(OPENFILENAME));

    ofn.lStructSize     = sizeof(OPENFILENAME);
    ofn.hwndOwner       = hWnd;
    ofn.lpstrFilter     = szFilter;
    ofn.nFilterIndex    = 1;
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = sizeof(szFile);
    ofn.lpstrFileTitle  = szFileTitle;
    ofn.nMaxFileTitle   = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) 
        return (ofn.lpstrFile); // LPSTR형의 화일 이름을 리턴한다.
    else // 화일 열기 에러  
         return NULL;   
	GetOpenFileName(
  LPOPENFILENAME lpofn   // initialization data

}*/
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
	DWORD		item_num;
	DWORD		i;
	DWORD		item_index[4000];
	CMeshFlag	meshFlag;
	CLightFlag	lightFlag;

	
	static UINT uiTimer;

	switch(message)
	{

	case WM_INITDIALOG:
		{
			g_hDlg = hdlg;
			g_pExp->GetMeshPropertyList()->SetDlgHandle(hdlg);
			
			g_pExp->DisplayObjectsList();

			
			char	szPriority[8];
			memset(szPriority,0,8);
			itoa(RENDER_ZPRIORITY_DEFAULT,szPriority,10);
			SetDlgItemText(g_hDlg,IDC_EDIT_RENDER_ZORDER,szPriority);

		}
		break;
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_CONVERT:



			g_pExp->Convert();
			g_pExp->WriteSceneFile();
			goto	lb_exit;
		
		case IDC_BUTTON_CHECK_ROOT_MODEL:
			{
				char	szFileName[_MAX_PATH];
				memset(szFileName,0,sizeof(szFileName));
				if (OpenFileDlg(szFileName,_MAX_PATH-1,g_hDlg, "MAX Custom Binary Exporter(*.MOD)|*.MOD|All Files(*.*)|*.*|"))
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

					if (0xffffffff == g_pGeometry->LoadModel(&g_pRootModel,szFileName,0))
					{
						MessageBox(NULL,"Fail to Load Model!","Error",MB_OK);
						return FALSE;
					}

					SetDlgItemText(g_hDlg,IDC_EDIT_ROOT_MODEL_FILE,szFileName);

					
				}

			}
			break;

		case IDC_BUTTON_OBJECT_APPLY:
			{
				item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
				
				if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_RADIO_LIGHT_LIGHT_MAP,BM_GETCHECK,0,0))
					meshFlag.SetShadeType(SHADE_TYPE_LIGHT_MAP);
				else if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_RADIO_LIGHT_VERTEX_LIGHT_RT,BM_GETCHECK,0,0))
					meshFlag.SetShadeType(SHADE_TYPE_VERTEX_LIGHT_RT);
				else if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_RADIO_LIGHT_VERTEX_LIGHT_IM,BM_GETCHECK,0,0))
					meshFlag.SetShadeType(SHADE_TYPE_VERTEX_LIGHT_IM);
				
//				if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_RADIO_VERTEX_ANI_DISABLE,BM_GETCHECK,0,0))
//					meshFlag.SetVertexAniType(VERTEX_ANIMATION_TYPE_DISABLE);
//				else if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_RADIO_VERTEX_ANI_ENABLE,BM_GETCHECK,0,0))
//					meshFlag.SetVertexAniType(VERTEX_ANIMATION_TYPE_ENABLE);
							
				if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_CHECK_MUST_ALIGN_VIEWPORT,BM_GETCHECK,0,0))
					meshFlag.SetTransformType(TRANSFORM_TYPE_ALIGN_VIEW);
				
		
				char	szPriority[8];
				memset(szPriority,0,8);
				GetDlgItemText(g_hDlg,IDC_EDIT_RENDER_ZORDER,szPriority,8);
				int	iPriority = atoi(szPriority);
				if (iPriority >= 64 || iPriority <= -64)
				{
					MessageBox(NULL,"Render Priority값은 +-63이내여야합니다.","Error",MB_OK);
					return FALSE;
				}
				meshFlag.SetRenderZPriorityValue(iPriority);
				iPriority = meshFlag.GetRenderZPriorityValue();

				if (BST_CHECKED == SendDlgItemMessage(hdlg,IDC_CHECK_DISABLE_ZBUFFER_WRITE,BM_GETCHECK,0,0))
					meshFlag.DisableZBufferWrite();
				else 
					meshFlag.EnableZBufferWrite();

				BOOL bZWrite = meshFlag.IsDisableZBubfferWrite();
				

				for (i=0; i<item_num; i++)
				{

					g_pExp->GetMeshPropertyList()->SetProperty(item_index[i],*(DWORD*)&meshFlag);
				}
			}
			break;
		}
		break;


	case WM_CLOSE:
		goto lb_exit;
		
	}
	return FALSE;
lb_exit:
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
		
//	CoFreeUnusedLibraries();
//	CoUninitialize();

	EndDialog(g_hDlg,1);
	return TRUE;
}
