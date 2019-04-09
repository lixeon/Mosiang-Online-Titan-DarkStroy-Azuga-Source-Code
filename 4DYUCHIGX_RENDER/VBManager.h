#pragma once

#include "../4DyuchiGRX_Common/stdafx.h"
#include "IResourcePool.h"

interface IResourceController;
class CVertexBuffer;

interface IVBManager : public IResourcePool
{
	virtual BOOL	__stdcall 	AllocVB(DWORD* pdwErrorCode,CVertexBuffer* pVertexBuffer,DWORD dwIndicesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController) = 0;
	virtual void	__stdcall 	ReleaseVB(void* pHandle) = 0;
	virtual void	__stdcall	ReleaseBufferAll() = 0;
	virtual void	__stdcall 	SetTimeStamp(void* pHandle) = 0;


};
