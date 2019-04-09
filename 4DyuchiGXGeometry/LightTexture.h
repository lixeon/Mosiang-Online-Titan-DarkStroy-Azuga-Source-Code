#pragma once

#include "../4DyuchiGRX_common/typedef.h"

#define LIGHTOP_ADD			0
#define	LIGHTOP_MUL			1

enum LIGHTTEXTURE_CRETAE_TYPE
{
	LIGHTTEXTURE_CREATE_TYPE_DEFULAT = 0x00000000,
	LIGHTTEXTURE_CREATE_TYPE_NOT_CREATE_PATCH = 0x00000001
	
};
struct FILE_LIGHT_TEXTURE_HEADER
{	
	DWORD			dwTexturePlaneNum;
	DWORD			dwSurfaceWidth;
	DWORD			dwSurfaceHeight;
};

class CLightTexture
{
	DWORD				m_dwTexturePlaneNum;
	DWORD				m_dwSurfaceWidth;
	DWORD				m_dwSurfaceHeight;

	TEXTURE_PLANE*		m_pTexturePlane;
	char*				m_pImageBuffer;
	DWORD				m_dwAlphaOP;
	BOOL				CreatePatch(IMAGE_DESC* pImageDesc);
	DWORD				ReadImageFile(char* pBits,void* pFP);
	void				OutLineToBorder(char* pBits);
	DWORD				WriteImageFile(char* pBits,FILE* fp);

	void				Cleanup();

public:
	BOOL				ReleasePatches();
	BOOL				GetLightTextureDesc(LIGHT_TEXTURE* pLightDesc);
	BOOL				CreateForHFieldObject(VECTOR3* pv3Rect,IMAGE_DESC* pImgDesc);
	BOOL				Duplicate(CLightTexture* pLightTexture);

	// 라이트맵 생성을 위한 함수들...
	BOOL				BeginCreateLightTexture(DWORD dwColor,DWORD dwFlag);
	BOOL				ShadeLightTextureAdd(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc);
	void				EndCreateLightTexture(IMAGE_DESC* pDesc,FILE* fp);
	void				ResetTransform(MATRIX4* pMatInverseWorld,MATRIX4* pMatResult);

	DWORD				GetSurfaceWidth()		{return m_dwSurfaceWidth;}
	DWORD				GetSurfaceHeight()		{return m_dwSurfaceHeight;}
	TEXTURE_PLANE*		GetTexturePlanePtr()	{return m_pTexturePlane;}
	DWORD				GetTexturePlaneNum()	{return m_dwTexturePlaneNum;}
	BOOL				SetPatchFromTextureImage(IMAGE_DESC* pImgDesc);
	BOOL				SetTextureImageFromPatch(IMAGE_DESC* pImgDesc);
	DWORD				ReadFile(void* pFP);
	DWORD				CreateLightMapImage(void* pFP,DWORD dwFlag);
	DWORD				WriteFile(FILE* fp);
	void				UpdatePatch(PATCH* pPatch);
	void				UpdateAll();

	CLightTexture();
	~CLightTexture();

};
