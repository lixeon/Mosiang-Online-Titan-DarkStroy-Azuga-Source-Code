#pragma once 

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/typedef.h"

BOOL StartConnectThread(DWORD dwMaxConnectNumAtSameTime);
BOOL Connect(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,CNetwork* pNetwork,void* pExt);
void EndConnectThread();
