#include "dialog.h"
#include "resource.h"
#include "anmexp.h"

HWND g_hDlg = NULL;
CAnmExp* g_pExp = NULL;
char*	g_pObjNameBuffer = NULL;


BOOL CALLBACK DlgProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam);


void SendTextToListbox(LPCTSTR msg,int id)
{
	SendDlgItemMessage(g_hDlg,id, LB_ADDSTRING, 0,(DWORD)msg);         

}
INT_PTR		CreatePanel(HINSTANCE hInst,HWND hWnd,CAnmExp* pExp)
{
	g_pExp = pExp;
	return DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_EXPORT),hWnd,DlgProc,(LPARAM)pExp);
}

BOOL CALLBACK DlgProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD		item_num;
	DWORD		i;
	DWORD		item_index[4000];
	CMotionFlag	motionFlag;
	CMotionFlag*	pMotionFlag;
	
	char		txt[128];

	switch(message)
	{

	case WM_INITDIALOG:
		{
			g_hDlg = hdlg;

			g_pExp->GetMotionPropertyList()->SetDlgHandle(hdlg);
			g_pExp->DisplayMotionObjectsList();


			wsprintf(txt,"%d",5);
			SetDlgItemText(hdlg,IDC_EDIT_KEY_SAMPLE_STEP,txt);
			SetDlgItemText(hdlg,IDC_EDIT_MESH_SAMPLE_STEP,txt);
			wsprintf(txt,"Unnamed Motion");
			SetDlgItemText(hdlg,IDC_EDIT_MOTION_NAME,txt);

//			HWND d = GetDlgItem(hdlg,IDC_EDIT_MSG);
//			EditBoxProc = (WNDPROC)SetWindowLong (d,GWL_WNDPROC,LONG(SubProc));

//			SendDlgItemMessage(hdlg,IDC_LIST_SERVER_STATUS, LB_SETHORIZONTALEXTENT,400,0);
//			SendDlgItemMessage(hdlg,IDC_LIST_USERS, LB_SETHORIZONTALEXTENT,400,0);
//			SendDlgItemMessage(hdlg,IDC_LIST_RECEIVED_MSG, LB_SETHORIZONTALEXTENT,400,0);
//			SendDlgItemMessage(hdlg,IDC_LIST_RECEIVED_MSG, LB_SETHORIZONTALEXTENT,400,0);

			//SendDlgItemMessage(hdlg,IDC_BUTTON_N,BM_SETSTATE,TRUE,0);
		}
		break;
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_EXPORT:
			g_pExp->WriteMotionFile();
			goto lb_exit;
		case IDC_BUTTON_KEY_ENABLE:
			item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
			
			for (i=0; i<item_num; i++)
			{
				pMotionFlag = (CMotionFlag*)g_pExp->GetMotionPropertyList()->GetPropertyPtr(item_index[i]);
				pMotionFlag->SetMotionTypeKeyFrame(MOTION_TYPE_KEYFRAME_ENABLE);
				
				g_pExp->GetMotionPropertyList()->DisplayItem(item_index[i]);
			}
			break;
		
		case IDC_BUTTON_KEY_DISABLE:
			item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
			
			for (i=0; i<item_num; i++)
			{
				pMotionFlag = (CMotionFlag*)g_pExp->GetMotionPropertyList()->GetPropertyPtr(item_index[i]);
				pMotionFlag->SetMotionTypeKeyFrame(MOTION_TYPE_KEYFRAME_DISABLE);
			
				g_pExp->GetMotionPropertyList()->DisplayItem(item_index[i]);
			}
			break;

		case IDC_BUTTON_VERTEX_ENABLE:
			item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
			
			for (i=0; i<item_num; i++)
			{
				pMotionFlag = (CMotionFlag*)g_pExp->GetMotionPropertyList()->GetPropertyPtr(item_index[i]);
				pMotionFlag->SetMotionTypeVertex(MOTION_TYPE_VERTEX_ENABLE);

				g_pExp->GetMotionPropertyList()->DisplayItem(item_index[i]);
			}
			break;
		
		case IDC_BUTTON_VERTEX_DISABLE:
			item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
			
			for (i=0; i<item_num; i++)
			{
				pMotionFlag = (CMotionFlag*)g_pExp->GetMotionPropertyList()->GetPropertyPtr(item_index[i]);
				pMotionFlag->SetMotionTypeVertex(MOTION_TYPE_VERTEX_DISABLE);
				
				g_pExp->GetMotionPropertyList()->DisplayItem(item_index[i]);
			}
			break;

				
		case IDC_BUTTON_UV_ENABLE:
			item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
			
			for (i=0; i<item_num; i++)
			{
				pMotionFlag = (CMotionFlag*)g_pExp->GetMotionPropertyList()->GetPropertyPtr(item_index[i]);
				pMotionFlag->SetMotionTypeUV(MOTION_TYPE_UV_ENABLE);

				g_pExp->GetMotionPropertyList()->DisplayItem(item_index[i]);
			}
			break;
		
		case IDC_BUTTON_UV_DISABLE:
			item_num = SendDlgItemMessage(hdlg,IDC_LIST_OBJECTS,LB_GETSELITEMS,4000,(LPARAM)item_index);
			
			for (i=0; i<item_num; i++)
			{
				pMotionFlag = (CMotionFlag*)g_pExp->GetMotionPropertyList()->GetPropertyPtr(item_index[i]);
				pMotionFlag->SetMotionTypeUV(MOTION_TYPE_UV_DISABLE);

				g_pExp->GetMotionPropertyList()->DisplayItem(item_index[i]);
			}
			break;
		}
		break;


	case WM_CLOSE:
		goto lb_exit;
		
	}
	return FALSE;
lb_exit:
	EndDialog(g_hDlg,1);
	return TRUE;

	//return DefDlgProc(hdlg,message,wParam,lParam);

}
