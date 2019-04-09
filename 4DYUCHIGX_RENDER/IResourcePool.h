#pragma once

#include "../4DyuchiGRX_Common/stdafx.h"

interface IResourcePool
{
	virtual void	__stdcall					OnLostDevice() = 0;
	virtual void	__stdcall					Reset() = 0;
	virtual void	__stdcall					Release() = 0;

	virtual	DWORD	__stdcall					GetResourceType() = 0;

};

