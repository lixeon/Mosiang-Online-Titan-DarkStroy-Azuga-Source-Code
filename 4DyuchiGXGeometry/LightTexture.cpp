#include "LightTexture.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"

CLightTexture::CLightTexture()
{
	memset(this,0,sizeof(CLightTexture));
	/*
	m_dwSurfaceWidth = 0;
	m_dwSurfaceHeight = 0;
	m_dwTexturePlaneNum = 0;
	m_pTexturePlane = NULL;*/

}



DWORD CLightTexture::ReadFile(void* pFP)
{
	FILE_LIGHT_TEXTURE_HEADER* pHeader = (FILE_LIGHT_TEXTURE_HEADER*)&this->m_dwTexturePlaneNum;
	
	DWORD	dwLen = 0;
	
	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_LIGHT_TEXTURE_HEADER));
	
	m_pTexturePlane = new TEXTURE_PLANE[m_dwTexturePlaneNum];

	dwLen += g_pFileStorage->FSRead(pFP,m_pTexturePlane,sizeof(TEXTURE_PLANE)*m_dwTexturePlaneNum);
	
	for (DWORD i=0; i<m_dwTexturePlaneNum; i++)
	{
		m_pTexturePlane[i].pPatch = NULL;
	}
	return dwLen;
}
DWORD CLightTexture::CreateLightMapImage(void* pFP,DWORD dwFlag)
{
	DWORD dwResult = 0;
	DWORD dwLen = 0;
	

	if (m_pImageBuffer)
		Cleanup();
	
	m_pImageBuffer = new char[m_dwSurfaceWidth*m_dwSurfaceHeight*2];
	memset(m_pImageBuffer,0xcc,m_dwSurfaceWidth*m_dwSurfaceHeight*2);
	
	if (pFP)
	{
		dwLen += ReadImageFile(m_pImageBuffer,pFP);
	
	}

//	if (m_pImageBuffer)
//		OutLineToBorder(m_pImageBuffer);

	IMAGE_DESC	imgDesc;
	imgDesc.imgHeader.dwBPS = 2;
	imgDesc.imgHeader.dwHeight = m_dwSurfaceHeight;
	imgDesc.imgHeader.dwWidth = m_dwSurfaceWidth;
	imgDesc.imgHeader.dwPitch = m_dwSurfaceWidth*2;
	imgDesc.pBits = m_pImageBuffer;


	if (!(dwFlag & LIGHTTEXTURE_CREATE_TYPE_NOT_CREATE_PATCH))
		CreatePatch(&imgDesc);
		
	return dwResult;

}
void CLightTexture::ResetTransform(MATRIX4* pMatInverseWorld,MATRIX4* pMatResult)
{
/*
		MATRIX4	matResult;
		MatrixMultiply2(&matResult,&m_TM.mat4Inverse,pResult);
		TransformVector3_VPTR1(m_pv3World,&matResult,m_dwVertexNum);
		*/


	DWORD		i;
	
	MATRIX4			matResult;
	MatrixMultiply2(&matResult,pMatInverseWorld,pMatResult);

	MATRIX4		matResultWithoutMove;
	matResultWithoutMove = matResult;
	matResultWithoutMove._41 = 0.0f;
	matResultWithoutMove._42 = 0.0f;
	matResultWithoutMove._43 = 0.0f;
	matResultWithoutMove._44 = 1.0f;

/*	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		for (j=0; j<m_pTexturePlane[i].dwPatchNum; j++)
		{	
			pPatch = m_pTexturePlane[i].pPatch+j;
			TransformVector3_VPTR1(&pPatch->v3Point,&matResult,1);
		}
	}*/
	
	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		if (m_pTexturePlane[i].pPatch)
		{
			delete [] m_pTexturePlane[i].pPatch;
			m_pTexturePlane[i].pPatch = NULL;
		}

		
		/*
		VECTOR3			v3Normal;
	VECTOR3			v3Rect[4];
	DWORD			dwEntryPosX;
	DWORD			dwEntryPosY;
	PLANE			plane;*/

		TransformVector3_VPTR1(m_pTexturePlane[i].v3Rect,&matResult,4);
		
		


		TransformVector3_VPTR1(&m_pTexturePlane[i].v3Normal,&matResultWithoutMove,1);
		TransformVector3_VPTR1(&m_pTexturePlane[i].plane.v3Up,&matResultWithoutMove,1);

		Normalize(&m_pTexturePlane[i].v3Normal,&m_pTexturePlane[i].v3Normal);
		CalcPlaneEquation(&m_pTexturePlane[i].plane,m_pTexturePlane[i].v3Rect);
	//	Normalize(&m_pTexturePlane[i].plane.v3Up,&m_pTexturePlane[i].plane.v3Up);

	
	}

	IMAGE_DESC	imgDesc;
	memset(&imgDesc,0,sizeof(imgDesc));
	CreatePatch(&imgDesc);

}
DWORD CLightTexture::WriteFile(FILE* fp)
{
	FILE_LIGHT_TEXTURE_HEADER* pHeader = (FILE_LIGHT_TEXTURE_HEADER*)&this->m_dwTexturePlaneNum;

	DWORD	oldPos = ftell(fp);
	
	fwrite(pHeader,sizeof(FILE_LIGHT_TEXTURE_HEADER),1,fp);
	fwrite(m_pTexturePlane,sizeof(TEXTURE_PLANE),m_dwTexturePlaneNum,fp);
	
	return (ftell(fp)-oldPos);
}
/*
BOOL CLightTexture::CreatePatch(IMAGE_DESC* pImgDesc)
{
	BOOL		bResult = FALSE;


	// 인자로 받은 비트맵은 2바이트짜리로 간주한다.

	VECTOR3		du,dv;
	PATCH*		pEntry;
	VECTOR3		normal;
	WORD		wColor;
	DWORD		A,R,G,B;

	DWORD		i,u,v;
	VECTOR3		v3Pitch;
		
	if (!m_dwTexturePlaneNum)
		goto lb_return;

	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		m_pTexturePlane[i].dwPatchNum = m_pTexturePlane[i].dwTexelSize*m_pTexturePlane[i].dwTexelSize;
		m_pTexturePlane[i].pPatch = new PATCH[m_pTexturePlane[i].dwPatchNum];
		memset(m_pTexturePlane[i].pPatch,0,sizeof(PATCH)*m_pTexturePlane[i].dwPatchNum);
		
		pEntry = m_pTexturePlane[i].pPatch;

//		du = (m_pTexturePlane[i].v3Rect[3] - m_pTexturePlane[i].v3Rect[0]);
//		dv = (m_pTexturePlane[i].v3Rect[1] - m_pTexturePlane[i].v3Rect[0]);

		VECTOR3_SUB_VECTOR3(&du,&m_pTexturePlane[i].v3Rect[3],&m_pTexturePlane[i].v3Rect[0]);
		VECTOR3_SUB_VECTOR3(&dv,&m_pTexturePlane[i].v3Rect[1],&m_pTexturePlane[i].v3Rect[0]);

//		v3Pitch = du / (float)m_pTexturePlane[i].dwTexelSize / 2.0f;
		float fTemp = (float)m_pTexturePlane[i].dwTexelSize;// / 2.0f;
		VECTOR3_DIV_FLOAT(&v3Pitch,&du,fTemp);
		
		
		

	//	CrossProduct(&normal,&du,&dv);
		Normalize(&normal,&m_pTexturePlane[i].v3Normal);

		for (v=0; v<m_pTexturePlane[i].dwTexelSize; v++)
		{
			for (u=0; u<m_pTexturePlane[i].dwTexelSize; u++)
			{
//				pEntry->v3Point = 
//					m_pTexturePlane[i].v3Rect[0] + 
//					(
///					du * (float)u / (float)dwTexelSize +
//					dv * (float)v / (float)dwTexelSize
//					) + v3Pitch;
				VECTOR3	v3PitchU,v3PitchV,v3PitchUV;
				float	fPitchU,fPitchV;

				fPitchU = (float)u / (float)m_pTexturePlane[i].dwTexelSize;
				fPitchV = (float)v / (float)m_pTexturePlane[i].dwTexelSize;
				
				VECTOR3_MUL_FLOAT(&v3PitchU,&du,fPitchU);
				VECTOR3_MUL_FLOAT(&v3PitchV,&dv,fPitchV);
				VECTOR3_ADD_VECTOR3(&v3PitchUV,&v3PitchU,&v3PitchV);
				VECTOR3_ADD_VECTOR3(&v3PitchUV,&v3PitchUV,&v3Pitch);
				VECTOR3_ADD_VECTOR3(&pEntry->v3Point,&m_pTexturePlane[i].v3Rect[0],&v3PitchUV);
	
	
				pEntry->sx = m_pTexturePlane[i].dwEntryPosX + u;
				pEntry->sy = m_pTexturePlane[i].dwEntryPosY + v;
				//pEntry->dwColor = 0xffffffff;

				if (pImgDesc->pBits)
				{
					wColor = *(WORD*)(pImgDesc->pBits + pEntry->sx*2 + pEntry->sy*pImgDesc->imgHeader.dwPitch);
					A = 0xff;
					R = (wColor & R_565_MASK)>>11<<3;
					G = (wColor & G_565_MASK)>>5<<2;
					B = (wColor & B_565_MASK)<<3;
			//		pEntry->dwColor = (A<<24) | (B<<16) | (G<<8) | R;
					pEntry->dwColor = (A<<24) | (R<<16) | (G<<8) | B;
				}
				else 
					pEntry->dwColor = 0x00000000;

				pEntry++;
			}
		}

	}
	bResult = TRUE;

lb_return:
	return bResult;
}
*/
BOOL CLightTexture::CreatePatch(IMAGE_DESC* pImgDesc)
{
	BOOL		bResult = FALSE;


	// 인자로 받은 비트맵은 2바이트짜리로 간주한다.

	PATCH*		pEntry;
	WORD		wColor;
	DWORD		A,R,G,B;

	DWORD		i,u,v;
	VECTOR3		v3PitchU;
	VECTOR3		v3PitchV;
	VECTOR3		v3PitchUV;
			
	if (!m_dwTexturePlaneNum)
		return FALSE;

	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
	//	m_pTexturePlane[i].dwEntryPosX--;
	//	m_pTexturePlane[i].dwEntryPosY--;
	//	m_pTexturePlane[i].dwTexelSize += 2;

		m_pTexturePlane[i].dwPatchNum = m_pTexturePlane[i].dwTexelSize*m_pTexturePlane[i].dwTexelSize;
		m_pTexturePlane[i].pPatch = new PATCH[m_pTexturePlane[i].dwPatchNum];
		memset(m_pTexturePlane[i].pPatch,0,sizeof(PATCH)*m_pTexturePlane[i].dwPatchNum);
		
		pEntry = m_pTexturePlane[i].pPatch;

//		du = (m_pTexturePlane[i].v3Rect[3] - m_pTexturePlane[i].v3Rect[0]);
//		dv = (m_pTexturePlane[i].v3Rect[1] - m_pTexturePlane[i].v3Rect[0]);

		VECTOR3		du,dv;
		VECTOR3_SUB_VECTOR3(&du,&m_pTexturePlane[i].v3Rect[3],&m_pTexturePlane[i].v3Rect[0]);
		VECTOR3_SUB_VECTOR3(&dv,&m_pTexturePlane[i].v3Rect[1],&m_pTexturePlane[i].v3Rect[0]);

//		m_pTexturePlane[i].v3Normal = plane.v3Up;
//		CrossProduct(&m_pTexturePlane[i].v3Normal,&du,&dv);
//		Normalize(&m_pTexturePlane[i].v3Normal,&m_pTexturePlane[i].v3Normal);
//		v3Pitch = du / (float)m_pTexturePlane[i].dwTexelSize / 2.0f;

		float fTemp = (float)m_pTexturePlane[i].dwTexelSize * 2.0f;
		VECTOR3_DIV_FLOAT(&v3PitchU,&du,fTemp);
		VECTOR3_DIV_FLOAT(&v3PitchV,&dv,fTemp);

		VECTOR3_ADD_VECTOR3(&v3PitchUV,&v3PitchU,&v3PitchV);

		
		
		for (v=0; v<m_pTexturePlane[i].dwTexelSize; v++)
		{
			for (u=0; u<m_pTexturePlane[i].dwTexelSize; u++)
			{
				pEntry->v3Point = 
					m_pTexturePlane[i].v3Rect[0] + 
					(
					du * (float)u / (float)m_pTexturePlane[i].dwTexelSize +
					dv * (float)v / (float)m_pTexturePlane[i].dwTexelSize
					) + v3PitchUV;
			
				pEntry->sx = (WORD)(m_pTexturePlane[i].dwEntryPosX + u);
				pEntry->sy = (WORD)(m_pTexturePlane[i].dwEntryPosY + v);
				//pEntry->dwColor = 0xffffffff;

				
				if (pImgDesc->pBits)
				{
					wColor = *(WORD*)(pImgDesc->pBits + pEntry->sx*2 + pEntry->sy*pImgDesc->imgHeader.dwPitch);
					A = 0xff;
					R = (wColor & R_565_MASK)>>11<<3;
					G = (wColor & G_565_MASK)>>5<<2;
					B = (wColor & B_565_MASK)<<3;
			//		pEntry->dwColor = (A<<24) | (B<<16) | (G<<8) | R;
					pEntry->dwColor = (A<<24) | (R<<16) | (G<<8) | B;
				}
				else 
					pEntry->dwColor = 0x00000000;

				pEntry++;
			}
		}

	}
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CLightTexture::GetLightTextureDesc(LIGHT_TEXTURE* pLightDesc)
{
	pLightDesc->pTexPlane = m_pTexturePlane;
	pLightDesc->dwTexPlaneNum = m_dwTexturePlaneNum;
	pLightDesc->dwSurfaceWidth = m_dwSurfaceWidth;
	pLightDesc->dwSurfaceHeight = m_dwSurfaceHeight;
	pLightDesc->pBits = m_pImageBuffer;
	pLightDesc->dwBPS = 2;

	return TRUE;
}

BOOL CLightTexture::BeginCreateLightTexture(DWORD dwColor,DWORD dwFlag)
{
	DWORD		i,j;
		
	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		for (j=0; j<m_pTexturePlane[i].dwPatchNum; j++)
		{
			if (dwFlag == LIGHTOP_MUL)
			{
				m_pTexturePlane[i].pPatch[j].dwLightColor[0] = dwColor & 0x000000ff;
				m_pTexturePlane[i].pPatch[j].dwLightColor[1] = (dwColor & 0x0000ff00)>>8;
				m_pTexturePlane[i].pPatch[j].dwLightColor[2] = (dwColor & 0x00ff0000)>>16;
				m_pTexturePlane[i].pPatch[j].dwLightColor[3] = 0x000000ff;
			}
			else
			{
				m_pTexturePlane[i].pPatch[j].dwColor = dwColor;
			}
		}
	}
	return TRUE;
}

BOOL CLightTexture::ShadeLightTextureAdd(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	float		cosang,distance;

	DWORD		lit_color[4];
	DWORD		light_color[4];

	DWORD		i,j,k;
	PLANE		wall;


	if (!pLight)
		return FALSE;

	m_dwAlphaOP = LIGHTOP_ADD;
		
	lit_color[0] = 0xff;
	PATCH*			pPatch;
	float			fFallOff;
	float			lightrsrs;
	float fDist,u,v;
	VECTOR3		v3RayDir,v3DirNormal;
	VECTOR3*	pv3Normal;

	lightrsrs = pLight->fRs * pLight->fRs;
	float		distdist;

//	VECTOR3		v3SamplePoint[9];
	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		VECTOR3		du,dv,v3PitchU,v3PitchV;
		VECTOR3_SUB_VECTOR3(&du,&m_pTexturePlane[i].v3Rect[3],&m_pTexturePlane[i].v3Rect[0]);
		VECTOR3_SUB_VECTOR3(&dv,&m_pTexturePlane[i].v3Rect[1],&m_pTexturePlane[i].v3Rect[0]);

		float fTemp = (float)m_pTexturePlane[i].dwTexelSize * 2.0f;
		VECTOR3_DIV_FLOAT(&v3PitchU,&du,fTemp);
		VECTOR3_DIV_FLOAT(&v3PitchV,&dv,fTemp);


		pv3Normal = &m_pTexturePlane[i].v3Normal;
	
		for (j=0; j<m_pTexturePlane[i].dwPatchNum; j++)
		{	
			pPatch = m_pTexturePlane[i].pPatch+j;
			
			lit_color[0] = pPatch->dwColor & 0x000000ff;
			lit_color[1] = (pPatch->dwColor & 0x0000ff00)>>8;
			lit_color[2] = (pPatch->dwColor & 0x00ff0000)>>16;
			
			distance = CalcDistance(&pPatch->v3Point,&pLight->v3Point);
			distdist = distance*distance;
			if (lightrsrs -  distdist <= 0)
				goto lb_next_patch;

//			if (distance > pLight->fRs)
//				goto lb_next_patch;

			VECTOR3_SUB_VECTOR3(&v3RayDir,&pLight->v3Point,&pPatch->v3Point);
			Normalize(&v3DirNormal,&v3RayDir);
			cosang = DotProduct(pv3Normal,&v3DirNormal);

		//	cosang += 0.8f;
		//	if (cosang < 0.0f)
		//		cosang *= (1.0f + cosang)*(1.0f + cosang);
			//	cosang *= -1.0f;

			if (cosang < 0.0f)
				goto lb_next_patch;

	//		if (cosang < 0.0f)
	//			cosang *= -1.0f;

			// create shadow...
			/*
			v3SamplePoint[0] = pPatch->v3Point - v3PitchU - v3PitchV;
			v3SamplePoint[1] = pPatch->v3Point - v3PitchU;
			v3SamplePoint[2] = pPatch->v3Point - v3PitchU + v3PitchV;

			v3SamplePoint[3] = pPatch->v3Point - v3PitchV;
			v3SamplePoint[4] = pPatch->v3Point + v3PitchV;

			v3SamplePoint[5] = pPatch->v3Point + v3PitchU - v3PitchV;
			v3SamplePoint[6] = pPatch->v3Point + v3PitchU;
			v3SamplePoint[7] = pPatch->v3Point + v3PitchU + v3PitchV;

			v3SamplePoint[8] = pPatch->v3Point;
*/
			for (k=0; k<dwFacesNum; k++)
			{
				CalcPlaneEquation(&wall,pv3+k*3);
				if (ComparePlane(&m_pTexturePlane[i].plane,&wall))
					goto lb_next_face;

				if (IntersectTriangle(NULL,&pPatch->v3Point,&v3RayDir,&pv3[k*3+0],&pv3[k*3+1],&pv3[k*3+2],&fDist,&u,&v,FALSE))
				{
					if (fDist*distance > ONE_CM && fDist < 1.0f)
						goto lb_next_patch;
				}
				
					
				/*	if (IntersectTriangle(NULL,&pPatch->v3Point,&v3RayDir,&pv3[k*3+0],&pv3[k*3+1],&pv3[k*3+2],&fDist,&u,&v,FALSE))
					{
						if (fDist*distance > ONE_CM && fDist < 1.0f)
							goto lb_next_patch;


						
					}*/

lb_next_face:
				__asm nop
			}

			
//			fFallOff = (pLight->fRs - distance) / pLight->fRs*0.7f + cosang*0.7f;
		
			
			light_color[0] = (pLight->dwDiffuse & 0x000000ff);
			light_color[1] = (pLight->dwDiffuse & 0x0000ff00)>>8;
			light_color[2] = (pLight->dwDiffuse & 0x00ff0000)>>16;
			
		

	//		fFallOff = (float)((lightrsrs -distdist) / lightrsrs)*0.7f + cosang*0.7f;
	//		fFallOff = (float)(lightrsrs - distance*distance) / lightrsrs + 0.3f;
//			fFallOff = (float)( (pLight->fRs - distance)*(pLight->fRs - distance)*1.3f ) / lightrsrs + cosang*0.7f;

			fFallOff = pFunc(pLight->fRs,lightrsrs,distance,distdist,cosang);

	
			

			for (k=0; k<3; k++)
			{
				lit_color[k] += (DWORD)(fFallOff*(float)light_color[k]);
				if (lit_color[k] > 0xff)
					lit_color[k] = 0xff;

			}
			pPatch->dwColor = (DWORD)(lit_color[2]<<16 | lit_color[1]<<8 | lit_color[0]);

		
lb_next_patch:
			__asm nop
		}

	}
	return TRUE;
}
void CLightTexture::EndCreateLightTexture(IMAGE_DESC* pDesc,FILE* fp)
{
	char*		pImageBuffer = new char[m_dwSurfaceWidth*m_dwSurfaceHeight*2];
	DWORD		i,j;
	WORD*		p = (WORD*)pImageBuffer;
	PATCH*		pPatch;
	DWORD		R,G,B;

	if (!m_dwTexturePlaneNum)
		return;
	

	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		for (j=0; j<m_pTexturePlane[i].dwPatchNum; j++)
		{

			pPatch = m_pTexturePlane[i].pPatch+j;
			R = (pPatch->dwColor & 0x00ff0000)>>16;
			G = (pPatch->dwColor & 0x0000ff00)>>8;
			B = (pPatch->dwColor & 0x000000ff);

			if (LIGHTOP_MUL & m_dwAlphaOP)
			{
				B = (DWORD)(( (float)B / 255.0f ) * ( ( float)pPatch->dwLightColor[0] / 255.0f ) * 2.0f ) * 255;
				G = (DWORD)(( (float)G / 255.0f ) * ( ( float)pPatch->dwLightColor[1] / 255.0f ) * 2.0f ) * 255;
				R = (DWORD)(( (float)R / 255.0f ) * ( ( float)pPatch->dwLightColor[2] / 255.0f ) * 2.0f ) * 255;
				
				if (B > 255)
					B = 255;
				if (G > 255)
					G = 255;
				if (R > 255)
					R = 255;

			}

			*(WORD*)((char*)p + pPatch->sx*2 + pPatch->sy*m_dwSurfaceWidth*2) = (WORD)
				(
				((R>>3)<<11) |
				((G>>2)<<5) |
				(B>>3)
				);
		}
	}

	if (pDesc)
	{
		memcpy(pDesc->pBits,pImageBuffer,m_dwSurfaceWidth*m_dwSurfaceHeight*2);
	}
//	WriteTGA("noborder.tga",pImageBuffer,m_dwSurfaceWidth,m_dwSurfaceHeight,2);
	OutLineToBorder(pImageBuffer);
	
	if (m_pImageBuffer)
	{
		memcpy(m_pImageBuffer,pImageBuffer,m_dwSurfaceWidth*m_dwSurfaceHeight*2);
	}
//	WriteTGA("border.tga",pImageBuffer,m_dwSurfaceWidth,m_dwSurfaceHeight,2);
	
//	__asm nop
//	if ((dwFlag & LIGHT_TEXTURE_CREATE_BORDER) == LIGHT_TEXTURE_CREATE_BORDER)
//	if ((dwFlag & LIGHT_TEXTURE_CREATE_PACK_FILE) != LIGHT_TEXTURE_CREATE_PACK_FILE)
//		WriteImageFile(pImageBuffer);

	if (fp)
	{
		WriteImageFile(pImageBuffer,fp);
	}
	delete [] pImageBuffer;
}

void CLightTexture::UpdateAll()
{
	char*		pImageBuffer = new char[m_dwSurfaceWidth*m_dwSurfaceHeight*2];
	DWORD		i,j;
	WORD*		p = (WORD*)pImageBuffer;
	PATCH*		pPatch;
	DWORD		R,G,B;

	if (!m_dwTexturePlaneNum)
		return;
	

	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		for (j=0; j<m_pTexturePlane[i].dwPatchNum; j++)
		{

			pPatch = m_pTexturePlane[i].pPatch+j;
			R = (pPatch->dwColor & 0x00ff0000)>>16;
			G = (pPatch->dwColor & 0x0000ff00)>>8;
			B = (pPatch->dwColor & 0x000000ff);

			
			*(WORD*)((char*)p + pPatch->sx*2 + pPatch->sy*m_dwSurfaceWidth*2) = (WORD)
				(
				((R>>3)<<11) |
				((G>>2)<<5) |
				(B>>3)
				);
		}
	}
	OutLineToBorder(pImageBuffer);
	memcpy(m_pImageBuffer,pImageBuffer,m_dwSurfaceWidth*m_dwSurfaceHeight*2);
	delete [] pImageBuffer;

}
void CLightTexture::UpdatePatch(PATCH* pPatch)
{
	if (!m_dwTexturePlaneNum)
		return;
	
	DWORD		R,G,B;


	R = (pPatch->dwColor & 0x00ff0000)>>16;
	G = (pPatch->dwColor & 0x0000ff00)>>8;
	B = (pPatch->dwColor & 0x000000ff);

			
	*(WORD*)((char*)m_pImageBuffer + pPatch->sx*2 + pPatch->sy*m_dwSurfaceWidth*2) = (WORD)
		(
		((R>>3)<<11) |
		((G>>2)<<5) |
		(B>>3)
		);

	OutLineToBorder(m_pImageBuffer);
}
void CLightTexture::OutLineToBorder(char* pBits)
{
	DWORD		i,y;
	char*		p = pBits;
	DWORD		entry_x,entry_y,size;

	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		entry_x = m_pTexturePlane[i].dwEntryPosX + 1;
		entry_y = m_pTexturePlane[i].dwEntryPosY + 1;
		size = m_pTexturePlane[i].dwTexelSize - 2;

		memcpy(
			pBits + entry_x*2 + (entry_y-1)*m_dwSurfaceWidth*2,
			pBits + entry_x*2 + entry_y*m_dwSurfaceWidth*2,
			size*2);

		memcpy(
			pBits + entry_x*2 + (entry_y+size)*m_dwSurfaceWidth*2,
			pBits + entry_x*2 + (entry_y+size-1)*m_dwSurfaceWidth*2,
			size*2);

		p = pBits + (entry_x-1)*2 + entry_y*m_dwSurfaceWidth*2;
		for (y=0; y<size; y++)
		{
			*(WORD*)p = *((WORD*)p+1);
			p += m_dwSurfaceWidth*2;
		}
		
		p = pBits + (entry_x+size)*2 + entry_y*m_dwSurfaceWidth*2;
		for (y=0; y<size; y++)
		{
			*(WORD*)p = *((WORD*)p-1);
			p += m_dwSurfaceWidth*2;
		}
		*(WORD*)(pBits + (entry_x-1)*2 + (entry_y-1)*m_dwSurfaceWidth*2)
			= *(WORD*)(pBits + (entry_x)*2 + (entry_y)*m_dwSurfaceWidth*2);

		*(WORD*)(pBits + (entry_x+size)*2 + (entry_y-1)*m_dwSurfaceWidth*2)
			= *(WORD*)(pBits + (entry_x+size-1)*2 + (entry_y)*m_dwSurfaceWidth*2);

		*(WORD*)(pBits + (entry_x-1)*2 + (entry_y+size)*m_dwSurfaceWidth*2)
			= *(WORD*)(pBits + (entry_x)*2 + (entry_y+size-1)*m_dwSurfaceWidth*2);

		*(WORD*)(pBits + (entry_x+size)*2 + (entry_y+size)*m_dwSurfaceWidth*2)
			= *(WORD*)(pBits + (entry_x+size-1)*2 + (entry_y+size-1)*m_dwSurfaceWidth*2);
	}
}
DWORD CLightTexture::ReadImageFile(char* pBits,void* pFP)
{

	FILE_LIGHT_TEXTURE_HEADER	header;
	
	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_LIGHT_TEXTURE_HEADER));
	dwLen += g_pFileStorage->FSRead(pFP,pBits,sizeof(char)*header.dwSurfaceWidth*header.dwSurfaceHeight*2);

	return dwLen;

}
BOOL CLightTexture::SetPatchFromTextureImage(IMAGE_DESC* pImgDesc)
{
	BOOL	bResult = FALSE;
	if (pImgDesc->imgHeader.dwBPS != 2)
		goto lb_return;

	if (pImgDesc->imgHeader.dwWidth != m_dwSurfaceWidth)
		goto lb_return;

	if (pImgDesc->imgHeader.dwHeight != m_dwSurfaceHeight)
		goto lb_return;


	// 인자로 받은 비트맵은 2바이트짜리로 간주한다.

	PATCH*		pEntry;

	WORD		wColor;
	DWORD		A,R,G,B;

	DWORD		i,u,v;
	
	if (!m_dwTexturePlaneNum)
		goto lb_return;


	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		pEntry = m_pTexturePlane[i].pPatch;

		for (v=0; v<m_pTexturePlane[i].dwTexelSize; v++)
		{
			for (u=0; u<m_pTexturePlane[i].dwTexelSize; u++)
			{
				wColor = *(WORD*)(pImgDesc->pBits + pEntry->sx*2 + pEntry->sy*m_dwSurfaceWidth*2);
					A = 0xff;
					R = (wColor & R_565_MASK)>>11<<3;
					G = (wColor & G_565_MASK)>>5<<2;
					B = (wColor & B_565_MASK)<<3;
					
//					pEntry->dwColor = (A<<24) | (B<<16) | (G<<8) | R;
					pEntry->dwColor = (A<<24) | (R<<16) | (G<<8) | B;
				pEntry++;
			}
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL CLightTexture::SetTextureImageFromPatch(IMAGE_DESC* pImgDesc)
{
	return TRUE;
}
DWORD CLightTexture::WriteImageFile(char* pBits,FILE* fp)
{
	DWORD	oldPos = ftell(fp);

	FILE_LIGHT_TEXTURE_HEADER* pHeader = (FILE_LIGHT_TEXTURE_HEADER*)&this->m_dwTexturePlaneNum;


	fwrite(pHeader,sizeof(FILE_LIGHT_TEXTURE_HEADER),1,fp);	
	fwrite(pBits,sizeof(char),m_dwSurfaceWidth*m_dwSurfaceHeight*2,fp);

	return (ftell(fp)-oldPos);
}

BOOL CLightTexture::Duplicate(CLightTexture* pLightTexture)
{
	DWORD	i;
	memcpy(pLightTexture,this,sizeof(FILE_LIGHT_TEXTURE_HEADER));

	pLightTexture->m_pTexturePlane = new TEXTURE_PLANE[m_dwTexturePlaneNum];
	memcpy(pLightTexture->m_pTexturePlane,m_pTexturePlane,sizeof(TEXTURE_PLANE)*m_dwTexturePlaneNum);

	if (m_pImageBuffer)
	{

		pLightTexture->m_pImageBuffer = new char[2*m_dwSurfaceWidth*m_dwSurfaceHeight];
		memcpy(pLightTexture->m_pImageBuffer,m_pImageBuffer,2*m_dwSurfaceWidth*m_dwSurfaceHeight);
	}
	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		if (m_pTexturePlane[i].pPatch)
		{
			pLightTexture->m_pTexturePlane[i].pPatch = new PATCH[m_pTexturePlane[i].dwPatchNum];
			memcpy(pLightTexture->m_pTexturePlane[i].pPatch,m_pTexturePlane[i].pPatch,sizeof(PATCH)*m_pTexturePlane[i].dwPatchNum);
			pLightTexture->m_pTexturePlane[i].dwPatchNum = m_pTexturePlane[i].dwPatchNum;
		}
		else
		{	pLightTexture->m_pTexturePlane[i].pPatch = NULL;
			pLightTexture->m_pTexturePlane[i].dwPatchNum = 0;
		}
	}
	return TRUE;
}
BOOL CLightTexture::ReleasePatches()
{
	BOOL	bResult = FALSE;

	if (!m_pTexturePlane)
		goto lb_return;

	DWORD	i;
	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		if (m_pTexturePlane[i].pPatch)
		{
			delete [] m_pTexturePlane[i].pPatch;
			m_pTexturePlane[i].pPatch = NULL;
		}
	}
	
	delete [] m_pTexturePlane;
	m_pTexturePlane = NULL;
	bResult = TRUE;

lb_return:
	return bResult;
}
void CLightTexture::Cleanup()
{
	if (m_pImageBuffer)
	{
		delete [] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	ReleasePatches();


}
CLightTexture::~CLightTexture()
{
	Cleanup();
}



/*
BOOL CLightTexture::CreateForHFieldObject(VECTOR3* pv3Rect,IMAGE_DESC* pImgDesc)
{
	m_dwTexturePlaneNum = 1;
	m_dwSurfaceWidth = pImgDesc->imgHeader.dwWidth;
	m_dwSurfaceHeight = pImgDesc->imgHeader.dwHeight;
	
	m_pTexturePlane = new TEXTURE_PLANE;
	memcpy(m_pTexturePlane->v3Rect,pv3Rect,sizeof(VECTOR3)*4);
	
	m_pTexturePlane->dwEntryPosX = 0;
	m_pTexturePlane->dwEntryPosY = 0;
	m_pTexturePlane->dwPatchNum = m_dwSurfaceWidth*m_dwSurfaceHeight;
	m_pTexturePlane->dwTexelSize = m_dwSurfaceWidth;
	
	CalcPlaneEquation(&m_pTexturePlane->plane,pv3Rect);
	m_pTexturePlane->v3Normal = m_pTexturePlane->plane.v3Up;

	CreatePatch(pImgDesc);
	
	return TRUE;
}*/

/*
BOOL CLightTexture::ShadeLightTextureMul(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight)
{


	float		cosang,distance;

	DWORD		light_color[4];
	DWORD		i,j,k;
	PLANE		wall;


	if (!pLight)
		return FALSE;

	m_dwAlphaOP = LIGHTOP_MUL;

	PATCH*			pPatch;

	float fDist,u,v;
	VECTOR3		v3RayDir,v3DirNormal;
	VECTOR3*	pv3Normal;

	for (i=0; i<m_dwTexturePlaneNum; i++)
	{
		pv3Normal = &m_pTexturePlane[i].v3Normal;
	
		for (j=0; j<m_pTexturePlane[i].dwPatchNum; j++)
		{	
			pPatch = m_pTexturePlane[i].pPatch+j;
			
			distance = CalcDistance(&pPatch->v3Point,&pLight->v3Point);
			if (distance > pLight->fRs)
				goto lb_next_patch;
		
			VECTOR3_SUB_VECTOR3(&v3DirNormal,&pLight->v3Point,&pPatch->v3Point);
			v3RayDir = v3DirNormal;
			Normalize(&v3DirNormal,&v3DirNormal);
			cosang = DotProduct(pv3Normal,&v3DirNormal);

			if (cosang < 0.0f)
				goto lb_next_patch;

			// create shadow...
			for (k=0; k<dwFacesNum; k++)
			{
				CalcPlaneEquation(&wall,pv3+k*3);
				if (!ComparePlane(&m_pTexturePlane[i].plane,&wall))
				{
					if (IntersectTriangle(&pPatch->v3Point,&v3RayDir,&pv3[k*3+0],&pv3[k*3+1],&pv3[k*3+2],&fDist,&u,&v))
					{
						if (fDist > 0.0f && fDist < 1.0f)
							goto lb_next_patch;
					}
				}
			}

			
			
			cosang = (pLight->fRs - distance) / pLight->fRs*0.7f + cosang*0.7f;
					
			
			light_color[0] = (pLight->dwColor & 0x000000ff);
			light_color[1] = (pLight->dwColor & 0x0000ff00)>>8;
			light_color[2] = (pLight->dwColor & 0x00ff0000)>>16;
						
			if (cosang > 0.0f)
			{
				for (k=0; k<3; k++)
				{
					pPatch->dwLightColor[k] += (DWORD)(cosang*(float)light_color[k]);
				}
			}
lb_next_patch:
			__asm nop
		}

	}
	return TRUE;
}*/
