#pragma once

#include "stdafx.h"
#include <initguid.h>
#include "console_type.h"
#include "iconsole_guid.h"

interface I4DyuchiCONSOLE : public IUnknown
{
	virtual	bool			__stdcall			CreateConsole(DESC_CONSOLE* desc) = 0;
	virtual bool			__stdcall			WriteDisplay(char* szMsg,DWORD dwLength,DWORD level) = 0;
	virtual bool			__stdcall			WriteFile(char* szMsg,DWORD dwLength,DWORD level) = 0;


	virtual void			__stdcall			SetDisplayRefreshRate(DWORD dwTick) = 0;
	virtual DWORD			__stdcall			GetDisplayRefreshRate() = 0;

	virtual void			__stdcall			RefreshDisplay() = 0;
	virtual void			__stdcall			FlushFileWrite() = 0;

	virtual void			__stdcall			SetDisplayFilterPriority(DWORD	level) = 0;
	virtual DWORD			__stdcall			GetDisplayFilterPriority() = 0;
	virtual void			__stdcall			SetWriteFileFilterPriority(DWORD level) = 0;
	virtual DWORD			__stdcall			GetWriteFileFilterPriority() = 0;

	
	virtual void			__stdcall			WaitMessage() = 0;
};
 
