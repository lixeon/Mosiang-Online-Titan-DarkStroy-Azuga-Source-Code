
#pragma once

#ifdef _MFC
	#include <afx.h>
	#include <ole2.h>
	#include <initguid.h>

#endif

#ifndef _MFC
	// Windows Header Files:
	#include <windows.h>
	#include <ole2.h>
	#include <initguid.h>

	// C RunTime Header Files
	#include <stdlib.h>
	#include <malloc.h>
	#include <memory.h>
	#include <tchar.h>
	#include <stdio.h>
#endif

#define GUID_SIZE 128
#define MAX_STRING_LENGTH 256
typedef void**	PPVOID;

typedef void** PPVOID;

