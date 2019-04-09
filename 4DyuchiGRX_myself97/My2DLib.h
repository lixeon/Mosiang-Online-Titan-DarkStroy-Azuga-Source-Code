#ifndef	_My2DLib_h_
#define _My2DLib_h_

#include <windows.h>

// 텍스쳐의 크기를 줄인다.
// pSrc는 원본 버퍼. pDest는 결과물 포인터. dwWidth는 원본 텍스쳐의 가로길이, dwHeight는 원본 텍스쳐의 높이,
//	dwScale은 몇배 줄일지(2면 1/2로 줄어든다.)
// dwWidth와 dwHeight는 언제나 2의 n승인걸 가정한다. 아니면 뻑.
// 비트 포멧은 565로 가정.
void ScaleDownTexture565( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);

// 4444 포멧.
void ScaleDownTexture4444( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);
// 8888 포맷 
void ScaleDownTexture8888(DWORD* pTex, DWORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);


#endif
