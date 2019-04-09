#include "geometry_global.h"
#include "../4DyuchiGXGFunc/global.h"
#include "Material.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"


/*
MATERIAL_TABLE_LIST* CreateMaterialTableFromFile(char* szFileName)
{
	DWORD	dwResult = 0;
	FILE*	fp = fopen(szFileName,"rb");
	if (!fp)
		goto lb_return;

	MTL_HEADER	header;
	fread(&header,sizeof(header),1,fp);
	
//	struct MATERIAL_TABLE_LIST
//{
//	DWORD			dwRefCount;
//	DWORD			dwNum;
//	MATERIAL_TABLE*	pMtlTable;
//};
	// 매터리얼 리스트를 할당할때 new,delete를 한방씩에 끝내도록...
	MATERIAL_TABLE_LIST*	pMtlTableList;
	char*		pEntry;
	CMaterial*	pMaterialEntry;

	pEntry = (char*)malloc(sizeof(MATERIAL_TABLE_LIST)-4 + sizeof(MATERIAL_TABLE)*header.dwMtlNum + sizeof(CMaterial)*header.dwMtlNum);
	
	pMtlTableList = (MATERIAL_TABLE_LIST*)pEntry;
	pMaterialEntry = (CMaterial*)((char*)pMtlTableList->pMtlTable + sizeof(MATERIAL_TABLE)*header.dwMtlNum);
	
	DWORD	i;
	for (i=0; i<header.dwMtlNum; i++)
	{
		pMaterialEntry[i].ReadFile(fp);

		pMtlTableList->pMtlTable[i].pMtl = pMaterialEntry[i].GetMaterialInfo();
		pMtlTableList->pMtlTable[i].dwMtlIndex = pMaterialEntry[i].GetIndex();
	}
	pMtlTableList->dwNum = header.dwMtlNum;
	pMtlTableList->dwRefCount = 0;

	fclose(fp);

lb_return:
	return pMtlTableList;
}*/

void SkipMaterialInSTM(void* pFP)
{
	g_pFileStorage->FSSeek(pFP,sizeof(STM_MATERIAL),FSFILE_SEEK_CUR);

}
void SkipMaterialListInSTM(void* pFP)
{
	DWORD	dwMtlNum;
	g_pFileStorage->FSRead(pFP,&dwMtlNum,sizeof(DWORD));

	for (DWORD i=0; i<dwMtlNum; i++)
	{
		SkipMaterialInSTM(pFP);
	}
	
}
DWORD CreateMaterialTableFromFile(MATERIAL_TABLE** ppMtlTable,char* szFileName)
{
	DWORD	dwResult = 0;
	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////

		goto lb_return;
	}

	MTL_HEADER	header;
	g_pFileStorage->FSRead(pFP,&header,sizeof(header));

	// 매터리얼 리스트를 할당할때 new,delete를 한방씩에 끝내도록...
	MATERIAL_TABLE*	pMtlTable;
	char*		pEntry;
	CMaterial*	pMaterialEntry;

	pEntry = (char*)malloc(sizeof(MATERIAL_TABLE)*header.dwMtlNum + sizeof(CMaterial)*header.dwMtlNum);
	
	pMtlTable = (MATERIAL_TABLE*)pEntry;
	pMaterialEntry = (CMaterial*)(pEntry + sizeof(MATERIAL_TABLE)*header.dwMtlNum);
	
	DWORD	i;
	for (i=0; i<header.dwMtlNum; i++)
	{
		pMaterialEntry[i].ReadFile(pFP);

		pMtlTable[i].pMtl = pMaterialEntry[i].GetMaterialInfo();
		pMtlTable[i].dwMtlIndex = pMaterialEntry[i].GetIndex();
	}
	

	g_pFileStorage->FSCloseFile(pFP);
	dwResult = header.dwMtlNum;
	*ppMtlTable = pMtlTable;
lb_return:
	return dwResult;
}


void ReleaseMaterialTableFromFile(MATERIAL_TABLE* pMtlTable)
{
	free(pMtlTable);
}

void ColorOPMultiply(IMAGE_DESC* pImgDesc,DWORD dwColor)
{
	DWORD	color_arg1[4];
	DWORD	color_arg2[4];

	color_arg2[3] = 0xff;
	color_arg2[0] = dwColor & 0x000000ff;
	color_arg2[1] = (dwColor & 0x0000ff00)>>8;
	color_arg2[2] = (dwColor & 0x00ff0000)>>16;

	for (DWORD y=0; y<pImgDesc->imgHeader.dwHeight; y++)
	{
		for (DWORD x=0; x<pImgDesc->imgHeader.dwWidth; x++)
		{
			WORD wColor = *(WORD*)(pImgDesc->pBits + x*2 + y*pImgDesc->imgHeader.dwPitch);
			color_arg1[3] = 0xff;
			color_arg1[0] = (wColor & R_565_MASK)>>11<<3;
			color_arg1[1] = (wColor & G_565_MASK)>>5<<2;
			color_arg1[2] = (wColor & B_565_MASK)<<3;

			for (DWORD k=0; k<3; k++)
			{
				color_arg1[k] =  ( (float)color_arg1[k] / 255.0f ) * ( ( float)color_arg2[k] / 255.0f ) * 2.0f * 255;
				if (color_arg1[k] > 255)
					color_arg1[k] = 255;
			}


			*(WORD*)((char*)pImgDesc->pBits + x*2 + y*pImgDesc->imgHeader.dwPitch) = 
				(WORD)
				(
				((color_arg1[0]>>3)<<11) |
				((color_arg1[1]>>2)<<5) |
				(color_arg1[2]>>3)
				);
		}
	}
}
DWORD __stdcall ReadImage(void* pFP,IMAGE_DESC* pDesc)
{
	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,&pDesc->imgHeader,sizeof(IMAGE_HEADER));
	
	DWORD size = pDesc->imgHeader.dwWidth*pDesc->imgHeader.dwHeight*pDesc->imgHeader.dwBPS;
	pDesc->pBits = new char[size];

	dwLen += g_pFileStorage->FSRead(pFP,pDesc->pBits,sizeof(char)*size);
	
	return dwLen;
}
void __stdcall ReleaseImage(IMAGE_DESC* pDesc)
{
	delete [] pDesc->pBits;
}
DWORD __stdcall WriteImage(FILE* fp,IMAGE_DESC* pImgDesc)
{
	DWORD	oldPos = ftell(fp);

	fwrite(&pImgDesc->imgHeader,sizeof(IMAGE_HEADER),1,fp);
	fwrite(pImgDesc->pBits,sizeof(char),pImgDesc->imgHeader.dwWidth*pImgDesc->imgHeader.dwHeight*pImgDesc->imgHeader.dwBPS,fp);

	return (ftell(fp)-oldPos);
}
BOOL IsIncludeVertex(PLANE* pPlane,VECTOR3* pv3Point)
{
	BOOL	bResult = TRUE;
	float D = (pv3Point->x * pPlane->v3Up.x + pv3Point->y * pPlane->v3Up.y + pv3Point->z * pPlane->v3Up.z + pPlane->D);
	
	if (D < 0)
		D *= -1.0f;

	if (D <= 0.1f)
		goto lb_return;

	bResult = FALSE;

lb_return:
	return bResult;

}


void __declspec(naked) __stdcall MultipyColor(DWORD* pdwResult,DWORD dwColor1,DWORD dwColor2)
{
	__asm
	{
		push	edi
		push	esi
		push	ecx
		push	ebx
		push	eax

		
		mov		esi,esp
		mov		edi,dword ptr[esp+4+5*4]

		add		esi,8+5*4
				
		mov		ecx,4

lb_loop:
		movzx	eax,byte ptr[esi]
		movzx	ebx,byte ptr[esi+4]
		and		al,bl
		//		mul		bx

	//	or		ah,ah
	//	jz		lb_write
		
//		xor		eax,eax
//		dec		eax

lb_write:
		mov		byte ptr[edi],al
		add		esi,1
		add		edi,1

		loop	lb_loop
				
		pop		eax
		pop		ebx
		pop		ecx
		pop		esi
		pop		edi

		ret		4*3
	}
}

DWORD GetSuffixStringFromExt(char* szDest,char* szFileName,DWORD dwLenFromRight)
{
	char	szTempPath[_MAX_PATH];
	DWORD	dwResult = 0;
    DWORD dwLen = RemoveExt(szTempPath,szFileName);
	DWORD i,dwGetLen;

	if (dwLen < dwLenFromRight)
		goto lb_return;

	dwGetLen = dwLen - dwLenFromRight;

	for (i=0; i<dwLenFromRight; i++)
	{
		szDest[i] = szTempPath[dwGetLen+i];
	}
	szDest[i] = 0;
	dwResult = dwLenFromRight;

lb_return:
	return dwResult;
}
DWORD GetRightString(char* szDest,char* szSrc,DWORD dwLenFromRight)
{
	DWORD	i,dwResult = 0;
	DWORD dwLen = lstrlen(szSrc);
	char*	pRight = szSrc;

	if (dwLen <= dwLenFromRight)
		goto lb_return;

	pRight += (dwLen - dwLenFromRight);
	

	for (i=0; i<dwLenFromRight; i++)
	{
		szDest[i] = pRight[i];
		
	}
	szDest[i] = 0;
	dwResult = dwLenFromRight;

lb_return:
	return dwResult;
}
void __stdcall 	CreatePlane(PLANE* pPlane,VECTOR3* pv3Up,VECTOR3* pv3Point)
{
	Normalize(&pPlane->v3Up,pv3Up);
	
	pPlane->D = -(pPlane->v3Up.x * pv3Point->x + pPlane->v3Up.y * pv3Point->y + pPlane->v3Up.z * pv3Point->z);
}
void __stdcall ShadeVertexColor(DWORD* pdwVertexColor,VECTOR3* pv3PointList,VECTOR3* pv3Normal,DWORD dwVerticesNum,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	DWORD		i,j,k;
	DWORD		lit_color[4];
	DWORD		light_color[4];
	PLANE		wall;
	float		fDist,u,v;
	VECTOR3		v3RayDir,v3DirNormal;
	float		cosang,distance,distdist,fFallOff,lightrsrs;

	lit_color[0] = 0xff;

	lightrsrs = pLight->fRs * pLight->fRs;

	for (i=0; i<dwVerticesNum; i++)
	{
		lit_color[0] = pdwVertexColor[i] & 0x000000ff;
		lit_color[1] = (pdwVertexColor[i] & 0x0000ff00)>>8;
		lit_color[2] = (pdwVertexColor[i] & 0x00ff0000)>>16;

		distance = CalcDistance(&pv3PointList[i],&pLight->v3Point);
		distdist = distance*distance;

		if (lightrsrs -  distdist <= 0)
			goto lb_next_patch;

				
		VECTOR3_SUB_VECTOR3(&v3DirNormal,&pLight->v3Point,&pv3PointList[i]);
		v3RayDir = v3DirNormal;
		Normalize(&v3DirNormal,&v3DirNormal);
		cosang = DotProduct(&pv3Normal[i],&v3DirNormal);
		/*
		if (!(pv3Normal[i].x == 0.0f && 
			pv3Normal[i].y == 0.0f && 
			pv3Normal[i].z == 0.0f))
		{
			cosang = 0.8f;
		}*/


		if (cosang < 0.0f)
		{
			goto lb_next_patch;
		}

		if (pv3)
		{

			// create shadow...
			for (j=0; j<dwFacesNum; j++)
			{
				CalcPlaneEquation(&wall,pv3+j*3);
				if (!IsIncludeVertex(&wall,&pv3PointList[i]))
				{
					if (IntersectTriangle(NULL,&pv3PointList[i],&v3RayDir,&pv3[j*3+0],&pv3[j*3+1],&pv3[j*3+2],&fDist,&u,&v,TRUE))
					{
						if (fDist > 0.0f && fDist < 1.0f)
						{
							goto lb_next_patch;
						}
					}
				}		
			}
		}
	//	fFallOff = (float)(lightrsrs -distdist) / lightrsrs + cosang*0.7f;
	//	cosang = (pLight->fRs - distance) / pLight->fRs*0.7f + cosang*0.7f;
	
	//	fFallOff = (pLight->fRs - distance)*(pLight->fRs - distance) / lightrsrs * 1.3f + cosang*0.7f;
		fFallOff = pFunc(pLight->fRs,lightrsrs,distance,distdist,cosang);

		light_color[0] = (pLight->dwDiffuse & 0x000000ff);
		light_color[1] = (pLight->dwDiffuse & 0x0000ff00)>>8;
		light_color[2] = (pLight->dwDiffuse & 0x00ff0000)>>16;
		
		for (k=0; k<3; k++)
		{
			lit_color[k] += (DWORD)(fFallOff*(float)light_color[k]);
			if (lit_color[k] > 0xff)
					lit_color[k] = 0xff;
		}
		pdwVertexColor[i] = (DWORD)((lit_color[2]<<16) | (lit_color[1]<<8) | (lit_color[0]));
			//	m_pVertexColor[i] = (DWORD)((lit_color[2]<<24) | (lit_color[1]<<16) | (lit_color[0]<<8));
		
lb_next_patch:
		pdwVertexColor[i] |= 0xff000000;
		//	m_pVertexColor[i] |= 0x000000ff;
	}
}


/*
void __stdcall CalcInverseRotate(MATRIX4* pResult,MATRIX4* pTransformMat)
{
	VECTOR3		v3Front;
	
	v3Front.x = 0.0f;
	v3Front.y = 0.0f;
	v3Front.z = -1.0f;

	MATRIX4		matRot = *pTransformMat;
	matRot._41 = 0.0f;
	matRot._42 = 0.0f;
	matRot._43 = 0.0f;


	VECTOR3		v3Dir;
	TransformVector3_VPTR2(&v3Dir,&v3Front,&matRot,1);
	float ang = CalcAngle(&v3Front,&v3Dir);
	
	VECTOR3		v3Axis;
	CrossProduct(&v3Axis,&v3Dir,&v3Front);
	Normalize(&v3Axis,&v3Axis);
	if (ang == 0.0f )
		SetIdentityMatrix(pResult);
	else
		SetRotationMatrix(pResult,&v3Axis,-1.0f*ang);
}
*/