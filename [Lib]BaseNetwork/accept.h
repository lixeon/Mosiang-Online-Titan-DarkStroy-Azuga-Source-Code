#pragma once

#include "Stdafx.h"

BOOL StartAcceptThread(SOCKET sckListen,HWND hWnd,UINT uiMsg);
void EndAcceptThread();
