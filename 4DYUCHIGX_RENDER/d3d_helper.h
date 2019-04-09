#ifndef D3D_HELPER_H
#define D3D_HELPER_H

#include "renderer_typedef.h"
#include <d3d8.h>

class CIndexBuffer;
class CD3DResourceManager;




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




void RestoreLightTexture(LPDIRECT3DTEXTURE8 pLightTexture,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum);

void SetColorFactorShader(LPDIRECT3DDEVICE8 pDevice,D3DCOLOR color,DWORD dwTexStageIndex );
void RestoreTextureShader(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex);


HRESULT SetAdjustedProjectionMatrix( MATRIX4* mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane,
                                     FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
                                     FLOAT fVPHeight );
void SetFrustumMatrix( MATRIX4* mat, FLOAT fLeft, FLOAT fRight, FLOAT fTop,
                                     FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane );
void SetHFieldTexMatrix(MATRIX4* pMat,VECTOR3* pv3Rect);
void SetHFieldTexMatrix4(MATRIX4* pMat,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight,DWORD dwDirFlag);

void __stdcall EnableAlphaSetting(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex,DWORD dwAlpha);
void __stdcall BeginAlphaMeshObject(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStage,DWORD dwMtlAlpha,DWORD dwObjAlpha);
void __stdcall BeginAlphaMeshObjectPerStageIndex(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex,DWORD dwMtlAlpha,DWORD dwObjAlpha);
void __stdcall EndAlphaMeshObject(LPDIRECT3DDEVICE8 pDevice);




BOOL __stdcall CompareLightDesc(LIGHT_DESC* pLight1,LIGHT_DESC* pLight2,float fMinDistance);



class CoD3DDevice;
void OutputErrorMsg(CoD3DDevice* pRenderer,HRESULT hr,char* szMsg);

BOOL DIB24To16(BYTE* pIn, BYTE* pOut, int iWidth, int iHeight);

typedef HRESULT (__stdcall *OnAllocBufferFunc)(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);
typedef HRESULT (__stdcall *OnAllocTexture)(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,UINT Width,UINT Height,UINT Levels,D3DFORMAT Format,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);


HRESULT __stdcall AllocVBFunc(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);
HRESULT __stdcall AllocIBFunc(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);

BOOL InitializeShader(LPDIRECT3DDEVICE8 pDevice,SHADER_HANDLE* pHandle,DWORD* pdwDecl,char* szFileName,SHADER_TYPE type);
void ReleaseShader(LPDIRECT3DDEVICE8 pDevice,SHADER_HANDLE* pHandle);

#endif

//void EnableAlphaSetting(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex,DWORD dwAlpha);
//void DefaultAlphaSetting(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex);