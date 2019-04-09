#include "../4DyuchiGRX_Common/typedef.h"

struct MTL_HEADER
{
	DWORD	dwVersion;
	DWORD	dwMtlNum;
	DWORD	dwMaxMtlIndex;

};
DWORD CreateMaterialTableFromFile(MATERIAL_TABLE** ppMtlTable,char* szFileName);
void ReleaseMaterialTableFromFile(MATERIAL_TABLE* pMtlTable);

BOOL IsIncludeVertex(PLANE* pPlane,VECTOR3* pv3Point);


void ColorOPMultiply(IMAGE_DESC* pImgDesc,DWORD dwColor);
DWORD GetSuffixStringFromExt(char* szDest,char* szFileName,DWORD dwLenFromRight);
DWORD GetRightString(char* szDest,char* szSrc,DWORD dwLenFromRight);

void __stdcall  MultipyColor(DWORD* pdwResult,DWORD dwColor1,DWORD dwColor2);
void __stdcall 	CreatePlane(PLANE* pPlane,VECTOR3* pv3Up,VECTOR3* pv3Point);
//void __stdcall CalcInverseRotate(MATRIX4* pResult,MATRIX4* pTransformMat);

DWORD	__stdcall ReadImage(FILE* fp,IMAGE_DESC* pDesc);
DWORD	__stdcall WriteImage(FILE* fp,IMAGE_DESC* pImgDesc);
void	__stdcall ReleaseImage(IMAGE_DESC* pDesc);
void	__stdcall ShadeVertexColor(DWORD* pdwVertexColor,VECTOR3* pv3PointList,VECTOR3* pv3Normal,DWORD dwVerticesNum,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);

void SkipMaterialnSTM(void* pFP);
void SkipMaterialListInSTM(void* pFP);

