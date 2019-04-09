#include "stdafx.h"
#include "helper.h"


void CopyRect(char* pDest,DWORD dwDestWidth,POINT* pt,char* pSrc,DWORD dwSrcWidth,RECT* pRect,DWORD dwBytesPerPixel,DWORD dwNum)
{
	DWORD i,y;

	DWORD	width,height;

	for (i=0; i<dwNum; i++)
	{
		width = pRect[i].right - pRect[i].left;
		height = pRect[i].bottom - pRect[i].top;
		for (y=0; y<height; y++)
		{
			memcpy(pDest + ((y+pt[i].y)*dwDestWidth + pt[i].x)*dwBytesPerPixel,
				pSrc + ((y+pRect[i].top)*dwSrcWidth + pRect[i].left)*dwBytesPerPixel,
				width*dwBytesPerPixel);
		}
	}
}

BOOL OpenMODFileName(char* szFileName,DWORD dwMaxLen)
{

	DWORD	dwLen;

	// TODO: Add your command handler code here
	char szFilters[] = "Model Files (*.mod) | *.mod | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE,"mod","*.mod",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() == IDOK)
	{
		pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (pathname == "")
			return FALSE;
	}

	

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return FALSE;

	dwLen = lstrlen(filename);
	if (!dwLen)
		return FALSE;

	

	if (dwMaxLen < dwLen)
		return FALSE;

	
	lstrcpy(szFileName,filename);
	szFileName[dwLen] = 0;
	return TRUE;
}
BOOL OpenMODFCHRFileName(char* szFileName,DWORD dwMaxLen)
{
	// TODO: Add your command handler code here
	char szFilters[] = "Model Files(*.mod)|*.mod|Character Files(*.chr) |*.chr||";
	
	DWORD	dwLen;
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,szFilters, NULL); 
	
		if (dlg.DoModal() == IDOK)
	{
		pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (pathname == "")
			return FALSE;
	}

	

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return FALSE;

	dwLen = lstrlen(filename);
	if (!dwLen)
		return FALSE;

	

	if (dwMaxLen < dwLen)
		return FALSE;

	
	lstrcpy(szFileName,filename);
	szFileName[dwLen] = 0;
	return TRUE;
}
