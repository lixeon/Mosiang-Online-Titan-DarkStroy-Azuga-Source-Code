#pragma once

//#ifndef GLOBAL_H
//#define GLOBAL_H

#ifndef GLOBAL_FUNC_DLL
#define GLOBAL_FUNC_DLL extern "C" __declspec(dllimport) 
#endif

#include "../4DyuchiGRX_common/typedef.h"
#include "type.h"

//#include "../4DyuchiGRX_common/math.inl"

#define IN
#define OUT

#ifndef _MAX_EXP
	GLOBAL_FUNC_DLL float	__stdcall Sin(float x);
	GLOBAL_FUNC_DLL float	__stdcall Cos(float x);
	GLOBAL_FUNC_DLL float	__stdcall ACos(float x);
	GLOBAL_FUNC_DLL float	__stdcall ASin(float x);
#endif
GLOBAL_FUNC_DLL BOOL	__stdcall ClipRenderObjectWithViewVolume(VIEW_VOLUME* pVolume,COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwFlag);
GLOBAL_FUNC_DLL BOOL	__stdcall ClipBoundingBoxWithViewProjMatrix(MATRIX4* pMatViewProj,COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwFlag);
GLOBAL_FUNC_DLL DWORD	__stdcall ClipLightWithRS(LIGHT_INDEX_DESC* pbLightIndexArrayResult,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,LIGHT_DESC* pLightDescIn,DWORD dwLightNum,BYTE bStartIndex);

GLOBAL_FUNC_DLL BOOL	__stdcall CalcIntersectPointLineAndPlane(VECTOR3* pv3Result,PLANE* pPlane,VECTOR3* pv3From,VECTOR3* pv3To, float* pft);
GLOBAL_FUNC_DLL void	__stdcall SET_VECTOR3(VECTOR3* pv3,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADD_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUB_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MUL_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIV_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADDEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUBEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MULEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIVEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADDEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUBEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MULEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIVEQU_FLOAT(VECTOR3* pv3Result,float fVal);

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADD_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUB_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MUL_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIV_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);

GLOBAL_FUNC_DLL void	__stdcall FindNearestVertexOnLine(VECTOR3* OUT pv3Result ,float* OUT pT, VECTOR3* IN pv3From, VECTOR3* IN pv3To, VECTOR3* IN pv3Point);
GLOBAL_FUNC_DLL float	__stdcall VECTOR3Length(VECTOR3* pv3);
GLOBAL_FUNC_DLL void	__stdcall MakeViewVolume(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up, float fov, float fHVRatio, float fFar);
GLOBAL_FUNC_DLL void	__stdcall MakeViewVolumeWithOrtho(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up, float fWidth, float fHVRatio, float fFar);


GLOBAL_FUNC_DLL void	__stdcall CrossProduct(VECTOR3* r,VECTOR3* u,VECTOR3* v);
GLOBAL_FUNC_DLL float	__stdcall DotProduct(VECTOR3* pv3_0,VECTOR3* pv3_1);
GLOBAL_FUNC_DLL void	__stdcall Normalize(VECTOR3* OUT vn,VECTOR3* IN v);
GLOBAL_FUNC_DLL float	__stdcall CalcAngle(VECTOR3* vec1, VECTOR3* vec2);
GLOBAL_FUNC_DLL float	__stdcall CalcCosAngle(VECTOR3* vec1, VECTOR3* vec2);
GLOBAL_FUNC_DLL float	__stdcall CalcDistance(VECTOR3* pv3Start,VECTOR3* pv3End);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionSpherAndSphere(VECTOR3* pv3PointTarget,float fRsTarget,VECTOR3* pv3PointSrc,float fRsSrc);
GLOBAL_FUNC_DLL BOOL	__stdcall SetIdentityMatrix(MATRIX4* pMat);
GLOBAL_FUNC_DLL BOOL	__stdcall SetInverseMatrix(MATRIX4* q, MATRIX4* a );

GLOBAL_FUNC_DLL void	__stdcall TransformVector3_VPTR1(VECTOR3* pv3SrcDest, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformVector3_VPTR2(VECTOR3* pv3Dest,VECTOR3* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformVector4_VPTR2(VECTOR4* pv3Dest,VECTOR4* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformV3TOV4(VECTOR4* pv3Dest,VECTOR3* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformNSizeVector3_VPTR1(char* pv3SrcDest,DWORD dwSize,MATRIX4* pMat,DWORD dwNum);

GLOBAL_FUNC_DLL void	__stdcall SetRotationMatrix(MATRIX4* mat, VECTOR3* vDir, FLOAT fRads );
GLOBAL_FUNC_DLL void	__stdcall SetRotationXMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	__stdcall SetRotationYMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	__stdcall SetRotationZMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	__stdcall MatrixMultiply3(MATRIX4* pResult,MATRIX4* mat1, MATRIX4* mat2, MATRIX4* mat3);
GLOBAL_FUNC_DLL void	__stdcall MatrixMultiply2(MATRIX4* pResult, MATRIX4* mat1, MATRIX4* mat2);
GLOBAL_FUNC_DLL void	__stdcall TranslateMatrix(MATRIX4* m, VECTOR3* vec);
GLOBAL_FUNC_DLL void	__stdcall SetScaleMatrix(MATRIX4* m, VECTOR3* v );
GLOBAL_FUNC_DLL void	__stdcall TransposeMatrix(MATRIX4* pMat);
GLOBAL_FUNC_DLL void	__stdcall SetLightTexMatrix(MATRIX4* pMat,VECTOR3* pv3Pos,float fRs);
GLOBAL_FUNC_DLL VOID	__stdcall MatrixFromQuaternion(MATRIX4* mat, QUARTERNION* q);
GLOBAL_FUNC_DLL VOID	__stdcall QuaternionSlerp(QUARTERNION* qQ,QUARTERNION* aQ,QUARTERNION* bQ,float fAlpha);
GLOBAL_FUNC_DLL BOOL	__stdcall CalcPlaneEquation(PLANE* pPlane,VECTOR3* pv3Tri);
GLOBAL_FUNC_DLL BOOL	__stdcall ComparePlane(PLANE* pTarget,PLANE* pSrc);
GLOBAL_FUNC_DLL BOOL	__stdcall CalcBoundingMesh(VECTOR3* pv3,DWORD dwVertexNum,COLLISION_MESH_OBJECT_DESC* pDesc);
GLOBAL_FUNC_DLL void	__stdcall CreateVertexListWithBox(char* pv3Array36,DWORD dwVertexSize,VECTOR3* pv3Oct);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionSphereAndPlane(VECTOR3* pv3Point,float fRs,PLANE* pPlane);
GLOBAL_FUNC_DLL void	__stdcall CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosY,float top,float left,float bottom,float right,float width,float height,DWORD dwFacesNumX,DWORD dwFacesNumY);

GLOBAL_FUNC_DLL void	__stdcall Set2PowValueLess(DWORD* pdwOut,DWORD dwIn);
GLOBAL_FUNC_DLL BOOL	__stdcall TransformBoundingSphere(BOUNDING_SPHERE* pDest,BOUNDING_SPHERE* pSrc,DWORD dwFlag);
GLOBAL_FUNC_DLL DWORD	__stdcall COLORtoDWORD(float r,float g,float b,float a);
GLOBAL_FUNC_DLL void	__stdcall QuaternionFromRotation(QUARTERNION* pQ,VECTOR3* v, float fTheta );
GLOBAL_FUNC_DLL void	__stdcall QuaternionMultiply(QUARTERNION* pQResult,QUARTERNION* pQA,QUARTERNION* pQB);

GLOBAL_FUNC_DLL BOOL	__stdcall AddBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDesc,COLLISION_MESH_OBJECT_DESC** ppDesc,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall DWORDtoCOLOR(DWORD color,float* r,float* g,float* b,float* a);
GLOBAL_FUNC_DLL	void	__stdcall PhysiqueTransform(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL	void	__stdcall PhysiqueTransformPosAndWriteUV(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,TVERTEX* ptv,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL void	__stdcall PhysiqueTransformPosAndNormalAndUV(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,TVERTEX* ptv,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL	void	__stdcall PhysiqueTransformPosAndNormal(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL void	__stdcall SetFaceNormalToVertex(VECTOR3* pv3NormalResult,VECTOR3* pv3List,DWORD dwVertexNum,WORD* pIndex,DWORD dwFacesNum);
GLOBAL_FUNC_DLL void	__stdcall SetFaceTangentToVertex(VECTOR3* pv3TangentResult,VECTOR3* pv3List,TVERTEX* ptvList,DWORD dwVertexNum,WORD* pIndex,DWORD dwFacesNum);

GLOBAL_FUNC_DLL BOOL	__stdcall RemoveCRLF(char* pStr,DWORD dwLen);
GLOBAL_FUNC_DLL BOOL	__stdcall IntersectTriangle(VECTOR3* pv3IntersectPoint,VECTOR3* orig,VECTOR3* dir,VECTOR3* v0,VECTOR3* v1, VECTOR3* v2,float* t, float* u, float* v,BOOL bCullBackface);
GLOBAL_FUNC_DLL void	__stdcall CalcRay(VECTOR3* pv3Pos,VECTOR3* pv3Dir,DWORD dwPosX,DWORD dwPosY,DWORD dwWidth,DWORD dwHeight,MATRIX4* pProj,MATRIX4* pView);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionMeshAndRay(VECTOR3* pv3IntersectPoint,float* pfDist,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,VECTOR3* pv3Pos,VECTOR3* pv3Dir);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionMeshWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,MATRIX4* pMatView,MATRIX4* pMatProj,POINT* ptCursor,SHORT_RECT* pClipRect);
GLOBAL_FUNC_DLL void	__stdcall SetTransformBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDest,COLLISION_MESH_OBJECT_DESC* pSrc,MATRIX4* pMat);
GLOBAL_FUNC_DLL void	__stdcall ResetTM(NODE_TM* pTM);
GLOBAL_FUNC_DLL BOOL	__stdcall SetViewMatrix(MATRIX4* pMat,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up);
GLOBAL_FUNC_DLL BOOL	__stdcall SetProjectionMatrix(MATRIX4* pMat,float fFOV,float fAspect,float fNearPlane,float fFarPlane);
GLOBAL_FUNC_DLL void	__stdcall SetBitmapFromTexturePlane(char* pBits,DWORD dwPitch,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum);
GLOBAL_FUNC_DLL BOOL	__stdcall WriteTGA(char* szFileName,char* pSrc,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,DWORD bpp);

GLOBAL_FUNC_DLL BOOL	__stdcall ResizeImage(char* pDest,DWORD dwDestWidth,DWORD dwDestHeight,char* pSrc,DWORD dwSrcWidth,DWORD dwSrcHeight,DWORD dwBytesPerPixel);
GLOBAL_FUNC_DLL DWORD	__stdcall CullBackFace(WORD* pFaceIndexList,TRI_FACE* pFaceEntry,VECTOR3* pv3Eye,DWORD dwFacesNum);
GLOBAL_FUNC_DLL DWORD	__stdcall BuildIndexBufferWithFaceIndexList(WORD* pIndexBufferEntry,TRI_FACE* pFaceEntry,WORD* pwFaceIndexEntry,VECTOR3* pv3Eye,DWORD dwFacesNum);
GLOBAL_FUNC_DLL DWORD	__stdcall BuildIndexBufferWithFaceIndexListForShadow(WORD* pIndexBufferEntry,TRI_FACE* pFaceEntry,WORD* pwFaceIndexEntry,VECTOR3* pv3Eye,BOUNDING_SPHERE* pSphere,DWORD dwFacesNum);
GLOBAL_FUNC_DLL void	__stdcall BuildFaceList(TRI_FACE* pFacesEntry,VECTOR3* pv3,WORD* pIndex,DWORD dwFacesNum);
GLOBAL_FUNC_DLL BOOL	__stdcall MakeViewVolumeIncludingSphere(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From, BOUNDING_SPHERE* pBS, float fFar);
GLOBAL_FUNC_DLL void	__stdcall MakeViewVolumeWithSelectRange(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From, VECTOR3* pv3LeftUp, VECTOR3* pv3RightUp, VECTOR3* pv3RightDown, VECTOR3* pv3LeftDown);


GLOBAL_FUNC_DLL void	__stdcall ConvertFromTo2YPR(VECTOR3* pVEC, VECTOR3* pOutYPR);
GLOBAL_FUNC_DLL	void	__stdcall RotatePositionWithPivot( VECTOR3* pOutPos, VECTOR3* pPivot, VECTOR3* pPosition, VECTOR3* pCameraAnglePYR, float fRotateYaw, float fRotatePitch);

GLOBAL_FUNC_DLL	float	__stdcall DistanceFromSphereAndPlane( BOUNDING_SPHERE* pSphere, PLANE* pPlane);
GLOBAL_FUNC_DLL	BOOL	__stdcall IsCollisionSphereAndBox( BOUNDING_SPHERE* pSphere, BOUNDING_BOX* pBox);

// ItemQueue


GLOBAL_FUNC_DLL ITEMQ_HANDLE	__stdcall	CreateItemQueue();
GLOBAL_FUNC_DLL void			__stdcall	ReleaseItemQueue(ITEMQ_HANDLE hQ);

GLOBAL_FUNC_DLL BOOL			__stdcall	BeginInitialize(ITEMQ_HANDLE pQ,DWORD dwMaxTypeNum);
GLOBAL_FUNC_DLL BOOL			__stdcall	SetType(ITEMQ_HANDLE pQ,DWORD dwTypeID,DWORD dwItemSize,void* pFunc,DWORD dwMaxNum);
GLOBAL_FUNC_DLL void			__stdcall	EndInitialize(ITEMQ_HANDLE pQ);

GLOBAL_FUNC_DLL BOOL			__stdcall	PushItem(ITEMQ_HANDLE pQ,DWORD dwTypeID,char* pItem);
GLOBAL_FUNC_DLL char*			__stdcall	GetItemSeq(ITEMQ_HANDLE pQ,DWORD* pdwTypeID,void** ppFunc);
GLOBAL_FUNC_DLL char*			__stdcall	GetItem(ITEMQ_HANDLE pQ,void** ppFunc,DWORD dwTypeID,DWORD dwSeqIndex);

GLOBAL_FUNC_DLL void			__stdcall	Clear(ITEMQ_HANDLE pQ);


GLOBAL_FUNC_DLL DWORD			__stdcall 	GetItemNum(ITEMQ_HANDLE pQ);
GLOBAL_FUNC_DLL DWORD			__stdcall	GetItemNumWithType(ITEMQ_HANDLE pQ,DWORD dwTypeID);
GLOBAL_FUNC_DLL void			__stdcall	ResetReadPosition(ITEMQ_HANDLE pQ);
	


// LookAsideList

GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	CreateStaticMemoryPool();
GLOBAL_FUNC_DLL void			__stdcall	ReleaseStaticMemoryPool(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL BOOL			__stdcall	InitializeStaticMemoryPool(STMPOOL_HANDLE pool,DWORD dwUnitSize,DWORD dwDefaultCommitNum,DWORD dwMaxNum);

GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);
GLOBAL_FUNC_DLL void			__stdcall	LALFreeWithCheck(STMPOOL_HANDLE pool,void* pMemory);
GLOBAL_FUNC_DLL void			__stdcall	LALEnableBlockCheck(STMPOOL_HANDLE pool,BOOL bSwitch);
GLOBAL_FUNC_DLL DWORD			__stdcall	LALGetAllocatedNum(STMPOOL_HANDLE pool);



// Various Bytes HashTable
GLOBAL_FUNC_DLL DWORD			__stdcall	VBHGetAllItem(VBHASH_HANDLE pHash,DWORD* pdwItemList,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL	DWORD			__stdcall	VBHSelect(VBHASH_HANDLE pHash,DWORD OUT* pItems,DWORD dwMaxItemNum,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL	void*			__stdcall	VBHInsert(VBHASH_HANDLE pHash,DWORD dwItem,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL	BOOL			__stdcall	VBHDelete(VBHASH_HANDLE pHash,void* pBucket);
typedef void (*DelFunc)(void* pVoid);
GLOBAL_FUNC_DLL DWORD			__stdcall	VBHDeleteWithKey(VBHASH_HANDLE pHash,void* pKeyData,DWORD dwSize,DelFunc pDelFunc);



GLOBAL_FUNC_DLL void*			__stdcall	VBHGetSearchHandle(VBHASH_HANDLE pHash,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL void*			__stdcall	VBHGetKeyPtr(void* pSearchHandle);

GLOBAL_FUNC_DLL	VBHASH_HANDLE	__stdcall	VBHCreate();
GLOBAL_FUNC_DLL	BOOL			__stdcall	VBHInitialize(VBHASH_HANDLE pHash,DWORD dwMaxBucketNum,DWORD dwMaxSize,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL	void			__stdcall	VBHRelease(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	DWORD			__stdcall	VBHGetMaxBucketNum(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL void			__stdcall	VBHDeleteAll(VBHASH_HANDLE pHash);

GLOBAL_FUNC_DLL DWORD			__stdcall	QBHSelect(QBHASH_HANDLE pHash,void** ppHashHandle,DWORD OUT* pItems,DWORD dwMaxItemNum,DWORD dwKeyData);
GLOBAL_FUNC_DLL void*			__stdcall	QBHInsert(QBHASH_HANDLE pHash,DWORD dwItem,DWORD dwKeyData);
GLOBAL_FUNC_DLL BOOL			__stdcall 	QBHDelete(QBHASH_HANDLE pHash,void* pBucket);

GLOBAL_FUNC_DLL	QBHASH_HANDLE	__stdcall	QBHCreate();
GLOBAL_FUNC_DLL BOOL			__stdcall	QBHInitialize(QBHASH_HANDLE pHash,DWORD dwMaxBucketNum,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL	void			__stdcall	QBHRelease(QBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	DWORD			__stdcall	QBHGetMaxBucketNum(QBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	void			__stdcall	QBDeleteAll(QBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL DWORD			__stdcall	QBHGetAllItem(VBHASH_HANDLE pHash,DWORD* pdwItemList,DWORD dwMaxItemNum);



// IndexCreattor
GLOBAL_FUNC_DLL		INDEXCR_HANDLE		__stdcall	ICCreate();
GLOBAL_FUNC_DLL		void				__stdcall	ICRelease(INDEXCR_HANDLE pIC);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ICAllocIndex(INDEXCR_HANDLE pIC);
GLOBAL_FUNC_DLL		void				__stdcall  	ICFreeIndex(INDEXCR_HANDLE pIC,DWORD dwIndex);
GLOBAL_FUNC_DLL		BOOL				__stdcall 	ICInitialize(INDEXCR_HANDLE pIC,DWORD dwMaxNum);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ICGetIndexNum(INDEXCR_HANDLE pIC);

// ItemIndexTable
GLOBAL_FUNC_DLL		ITEMTABLE_HANDLE	__stdcall	ITCreate();
GLOBAL_FUNC_DLL		void				__stdcall	ITRelease(ITEMTABLE_HANDLE pIT);

GLOBAL_FUNC_DLL		BOOL				__stdcall	ITInitialize(ITEMTABLE_HANDLE pIT,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ITAddItem(ITEMTABLE_HANDLE pIT,void* pVoid);
GLOBAL_FUNC_DLL		BOOL				__stdcall	ITDeleteItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ITGetItemNum(ITEMTABLE_HANDLE pIT);
GLOBAL_FUNC_DLL		void*				__stdcall	ITGetItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
GLOBAL_FUNC_DLL		void*				__stdcall	ITGetItemSequential(ITEMTABLE_HANDLE pIT,DWORD dwSeqIndex);

GLOBAL_FUNC_DLL		DWORD				__stdcall	RemoveExt(char* szResultName,char* szFileName);
GLOBAL_FUNC_DLL		DWORD				__stdcall	GetNameRemovePath(char* dest,char* src);
GLOBAL_FUNC_DLL		DWORD				__stdcall	GetNamePath(char* szResultPath,char* szFileName);
GLOBAL_FUNC_DLL		DWORD				__stdcall	GetExt(char* szResultName,char* szFileName);


// BinSearchAE
GLOBAL_FUNC_DLL		BSAE_HANDLE			__stdcall	BSAECreate();
GLOBAL_FUNC_DLL		DWORD				__stdcall	BSAEGetItemNum(BSAE_HANDLE pBSAE);
GLOBAL_FUNC_DLL		BOOL				__stdcall	BSAEInitialize(BSAE_HANDLE pBSAE,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL		ITEM_CONTAINER_AE*	__stdcall	BSAEInsertItem(BSAE_HANDLE pBSAE,DWORD dwKey,void* pItem);
GLOBAL_FUNC_DLL		BOOL				__stdcall	BSAEDeleteItem(BSAE_HANDLE pBSAE,ITEM_CONTAINER_AE* pItemContainer);
GLOBAL_FUNC_DLL		void*				__stdcall	BSAESearchItem(BSAE_HANDLE pBSAE,DWORD dwKey);
GLOBAL_FUNC_DLL		ITEM_CONTAINER_AE*	__stdcall	BSAESearchItemContainer(BSAE_HANDLE pBSAE,DWORD dwKey);
GLOBAL_FUNC_DLL		void				__stdcall	BSAERelease(BSAE_HANDLE pBSAE);

// various heap
GLOBAL_FUNC_DLL		VHEAP_HANDLE		__stdcall	VHPCreateHeap();
GLOBAL_FUNC_DLL		void				__stdcall	VHPHeapCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		void				__stdcall	VHPLeakCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		BOOL				__stdcall	VHPInitialize(VHEAP_HANDLE pVHeap,void* pMassMemory,DWORD dwMassMemorySize,DWORD dwMaxBlockNum,DWORD dwUnitSize,DWORD dwAlignUnitNum,BOOL bEnableDebug);

GLOBAL_FUNC_DLL		void*				__stdcall	VHPAlloc(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void* 				__stdcall	VHPAllocUnit(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwUnitNum,DWORD* pdwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void				__stdcall	VHPFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void*				__stdcall	VHPDBGAlloc(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void* 				__stdcall	VHPDBGAllocUnit(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwUnitNum,DWORD* pdwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void				__stdcall 	VHPDBGFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void				__stdcall	VHPReleaseHeap(VHEAP_HANDLE pVHeap);



// DPCQ
GLOBAL_FUNC_DLL		DPCQ_HANDLE			__stdcall	DPCQCreate();
GLOBAL_FUNC_DLL		BOOL				__stdcall	DPCQInitialize(DPCQ_HANDLE pDPCQHandle,DWORD dwMaxBufferSize);
GLOBAL_FUNC_DLL		BOOL				__stdcall	DPCQPushDPC(DPCQ_HANDLE pDPCQHandle,void* pStdcallFunc,WORD wArgNum,DWORD* pArgList,int* pRetAddr,int iOrder);
GLOBAL_FUNC_DLL		void				__stdcall	DPCQProcess(DPCQ_HANDLE pDPCQHandle);
GLOBAL_FUNC_DLL		void				__stdcall	DPCQProcessWithAscSort(DPCQ_HANDLE pDPCQHandle);

GLOBAL_FUNC_DLL		void				__stdcall	DPCQClear(DPCQ_HANDLE pDPCQHandle);
GLOBAL_FUNC_DLL		void				__stdcall	DPCQRelease(DPCQ_HANDLE pDPCQHandle);

// check performance
GLOBAL_FUNC_DLL void BeginAnalyze(PERFORMANCE_CONTEXT* pCtx);
GLOBAL_FUNC_DLL BOOL EndAnalyze(PERFORMANCE_CONTEXT* pCtx);

GLOBAL_FUNC_DLL void __stdcall GetEIP(DWORD* pdwEIP);

#define RDTSC_BEGIN		__asm	rdtsc			\
						__asm	push	eax		\
						__asm	push	edx		\


#define RDTSC_END		__asm	rdtsc			\
						__asm	pop		ebx		\
						__asm	pop		ecx		\
						__asm	sub		eax,ecx	\
						__asm	sub		edx,ebx	\
						__asm	int		3		\


//#endif
GLOBAL_FUNC_DLL BOOL __stdcall IsValidFloat(float f);
GLOBAL_FUNC_DLL BOOL __stdcall IsValidVector3(VECTOR3* pv3,DWORD dwNum);
GLOBAL_FUNC_DLL BOOL __stdcall IsValidTri(VECTOR3* pv3Point_0,VECTOR3* pv3Point_1,VECTOR3* pv3Point_2);

GLOBAL_FUNC_DLL void __stdcall QSortIntASC(SORT_ITEM_INT* pArray,DWORD dwNum);
GLOBAL_FUNC_DLL void __stdcall QSortIntDSC(SORT_ITEM_INT* pArray,DWORD dwNum);
GLOBAL_FUNC_DLL void __stdcall QSortFloatASC(SORT_ITEM_FLOAT* pArray,DWORD dwNum);
GLOBAL_FUNC_DLL void __stdcall QSortFloatDSC(SORT_ITEM_FLOAT* pArray,DWORD dwNum);


GLOBAL_FUNC_DLL void __stdcall CharToSmallASCII(char* szDest,char* szStr,DWORD dwLen);
GLOBAL_FUNC_DLL void __stdcall CharToSmallASCIILen(char* szDest,char* szStr);


GLOBAL_FUNC_DLL void __stdcall LinktToSortLink(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pNew);
GLOBAL_FUNC_DLL void __stdcall UnLinkFromSortLink(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pDel);


GLOBAL_FUNC_DLL STRCPOOL_HANDLE __stdcall CreateResourcePool();
GLOBAL_FUNC_DLL BOOL			__stdcall RCPInitialize(STRCPOOL_HANDLE pHandle,DWORD dwMaxItemNum,RESOURCR_ALLOC_FUNC pAllocFunc,RESOURCR_FREE_FUNC pFreeFunc);
GLOBAL_FUNC_DLL void*			__stdcall RCPAlloc(STRCPOOL_HANDLE pHandle,BOOL* pbEnable,DWORD* pdwResourceHandle,void* pArg1,void* pArg2,void* pArg3,DWORD dwSize);
GLOBAL_FUNC_DLL void			__stdcall RCPFree(STRCPOOL_HANDLE pHandle,DWORD dwResourceHandle);
GLOBAL_FUNC_DLL void			__stdcall RCPFreeAll(STRCPOOL_HANDLE pHandle);
GLOBAL_FUNC_DLL void			__stdcall RCPRelease(STRCPOOL_HANDLE pHandle);


GLOBAL_FUNC_DLL void __stdcall Convert8BitTo2BitImage(char* p2Bits,char* p8Bits,DWORD dwWidth,DWORD dwHeight);
GLOBAL_FUNC_DLL void __stdcall Convert2BitTo8BitImage(char* p8Bits,char* p2Bits,DWORD dwWidth,DWORD dwHeight);
GLOBAL_FUNC_DLL void __stdcall Convert2BitTo16BitImageWithPitch(char* p16Bits,char* p2Bits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch);
GLOBAL_FUNC_DLL void __stdcall SetPixel2BitImage(char* p2Bits,DWORD dwPx,DWORD dwPy,DWORD dwWidth,DWORD dwHeight,DWORD dwColor);
GLOBAL_FUNC_DLL void __stdcall SubPixel2BitImage(char* p2Bits,DWORD dwPx,DWORD dwPy,DWORD dwWidth,DWORD dwHeight,DWORD dwColor);
GLOBAL_FUNC_DLL void __stdcall AddPixel2BitImage(char* p2Bits,DWORD dwPx,DWORD dwPy,DWORD dwWidth,DWORD dwHeight,DWORD dwColor);
GLOBAL_FUNC_DLL void __stdcall Create8BitsAttenuationMap(char* p8Bits,DWORD dwWidth,DWORD dwHeight,DWORD dwRS);

GLOBAL_FUNC_DLL void __stdcall SetOutLineBorder4Bytes(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,DWORD dwBorderColor);
GLOBAL_FUNC_DLL void __stdcall SetOutLineBorder2Bytes(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,DWORD wBorderColor);

GLOBAL_FUNC_DLL DWORD	__stdcall	CollisionTestBoxToBox(VECTOR3* pv3Quad,DWORD* pdwAxisMask,BOUNDING_BOX* pBoxDest,BOUNDING_BOX* pBoxSrcA,BOUNDING_BOX* pBoxSrcB);
//GLOBAL_FUNC_DLL void	__stdcall	AdjustViewVolume(VIEW_VOLUME* pVVDest,VIEW_VOLUME* pVVSrc,CAMERA_DESC* pCameraDesc,PORTAL* pPortal);
//GLOBAL_FUNC_DLL BOOL	__stdcall	IsIncludePortal(VIEW_VOLUME* pVV,PORTAL* pPortal);
//GLOBAL_FUNC_DLL DWORD	__stdcall	SearchRoomWithCollisionMesh(PORTAL_ROOM** ppResultRoomList,PORTAL_ROOM** ppRoomList,DWORD dwRoomNum,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,DWORD dwMaxResultNum,BOOL* pbInsufficient);
