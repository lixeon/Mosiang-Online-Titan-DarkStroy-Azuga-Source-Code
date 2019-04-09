#pragma once

#include "RainFTP_TypeDefine.h"

static const GUID CLSID_RainFTP= 
{0xbd895bfe, 0xfae3, 0x40ad,{ 0x98, 0x60, 0x77, 0x16, 0x14, 0xa5, 0x2, 0xff}};

// {7DD5223F-CC9C-4c44-A330-F19F3443D16E}
static const GUID IID_RainFTP= 
{0xc1b6e43b, 0x2846, 0x4f7d, {0x82, 0x25, 0x41, 0x95, 0xbf, 0x90, 0x2, 0xc1}};


// {BD895BFE-FAE3-40ad-9860-771614A502FF}
// DEFINE_GUID( CLSID_RainFTP, 
// );
// 
// {C1B6E43B-2846-4f7d-8225-4195BF9002C1}
// DEFINE_GUID( IID_RainFTP, 
// );


interface IRainFTP : public IUnknown
{
	virtual int		__stdcall		Initialize			( HWND hwNotify, DWORD dwNotifyMessage, DWORD dwMaxSession)			= 0;
	virtual HANDLE	__stdcall		CreateSession		()																	= 0;
	virtual int		__stdcall		CloseSession		( HANDLE hSession)													= 0;

	virtual int		__stdcall		SetContext			( HANDLE hSession, PVOID pvContext)									= 0;
	virtual int		__stdcall		GetContext			( HANDLE hSession, PVOID* ppvContext)								= 0;

	//	Result, Control Command List
	virtual HANDLE	__stdcall		GetCompletionHandle	( PVOID* ppvContext = NULL)											= 0;
	virtual int		__stdcall		GetCompletionResult	( HANDLE hSession, PVOID* ppvCompletionData, BOOL bWait = FALSE)	= 0;
	virtual int		__stdcall		Shutdown			( HANDLE hSession)													= 0;

	//	FTP Command List
	virtual int		__stdcall		Connect				( HANDLE hSession, Connect_data* pCD)								= 0;
	virtual int		__stdcall		GetCurrentDirectory	( HANDLE hSession, Directory_data* pDD)								= 0;
	virtual int		__stdcall		SetCurrentDirectory	( HANDLE hSession, Directory_data* pDD)								= 0;
	virtual int		__stdcall		GetList				( HANDLE hSession, List_data* pLD)									= 0;
	virtual int		__stdcall		DownloadFile		( HANDLE hSession, File_data* pFD)									= 0;
};

typedef HRESULT (__stdcall *DllGetClassObject_RainFTP)( REFCLSID , REFIID , LPVOID*);
