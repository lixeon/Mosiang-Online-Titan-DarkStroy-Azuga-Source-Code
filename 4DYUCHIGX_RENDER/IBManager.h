#pragma once

#include "../4DyuchiGRX_Common/stdafx.h"
#include "IResourcePool.h"

class CIndexBuffer;
interface IResourceController;


interface IIBManager : public IResourcePool
{
	virtual	BOOL	__stdcall		AllocIB(DWORD* pdwErrorCode,CIndexBuffer* pIndexBuffer,DWORD dwIndicesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController) = 0;
	virtual void	__stdcall		ReleaseIB(void* pHandle) = 0;
	virtual void	__stdcall		ReleaseBufferAll() = 0;
};