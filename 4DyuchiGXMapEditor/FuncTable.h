#ifndef FUNC_TABLE_H
#define FUNC_TABLE_H

GXOSetTransformMatrix(GXOBJECT_HANDLE gxo,MATRIX4* pMat) = 0;
	virtual void				__stdcall	GXOSetCurrentFrame(GXOBJECT_HANDLE gxh,DWORD dwFrame) = 0;
	virtual void				__stdcall	GXOSetCurrentMotionIndex(GXOBJECT_HANDLE gxh,DWORD dwMotionIndex) = 0;
	virtual MOTION_DESC*		__stdcall	GXOGetMotionDesc(GXOBJECT_HANDLE gxh,DWORD dwMotionIndex) = 0;
	virtual DWORD				__stdcall	GXOGetMotionNum(GXOBJECT_HANDLE gxh) = 0;
	virtual DWORD				__stdcall	GXOGetCurrentFrame(GXOBJECT_HANDLE gxh) = 0;
	virtual DWORD				__stdcall	GXOGetMotionIndex(GXOBJECT_HANDLE gxh) = 0;
	virtual void				__stdcall	GXOSetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void				__stdcall	GXOMovePosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void				__stdcall	GXOGetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void				__stdcall	GXOSetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float fRad) = 0;
	virtual void				__stdcall	GXOGetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float* pfRad) = 0;
	virtual void				__stdcall	GXOMoveForward(GXOBJECT_HANDLE gxh,float fDistance) = 0;
	virtual void				__stdcall	EstimatedMoveForward(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos,float fDistance) = 0;

	virtual BOOL				__stdcall	GXOAttach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,char* szObjName) = 0;
	virtual BOOL				__stdcall	GXODetach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE child_gxh) = 0;
	virtual BOOL				__stdcall	GXOAttachCamera(GXOBJECT_HANDLE gxh,char* szCameraName,char* szCameraTargetName) = 0;
	virtual void				__stdcall	GXODetachCamera() = 0;
	virtual BOOL				__stdcall	GXOAttachLight(GXOBJECT_HANDLE gxoh,char* szObjName,GXLIGHT_HANDLE gxlh,DWORD dwAttachType) = 0;
	virtual BOOL				__stdcall	GXODetachLight(GXOBJECT_HANDLE gxoh,GXLIGHT_HANDLE child_gxh) = 0;
	

	virtual void				__stdcall	GXOSetRenderFlag(GXOBJECT_HANDLE gxoh,DWORD dwFlag) = 0;
	virtual	DWORD				__stdcall	GXOGetRenderFlag(GXOBJECT_HANDLE gxoh) = 0;

	virtual void				__stdcall	GXOSetAlphaFlag(GXOBJECT_HANDLE gxoh,DWORD dwFlag) = 0;
	virtual	DWORD				__stdcall	GXOGetAlphaFlag(GXOBJECT_HANDLE gxoh) = 0;
	
	virtual void				__stdcall 	GXODisableRender(GXOBJECT_HANDLE gxoh) = 0;
	virtual void				__stdcall 	GXOEnableRender(GXOBJECT_HANDLE gxoh) = 0;
	virtual void				__stdcall	GXODisableSchedule(GXOBJECT_HANDLE gxoh) = 0;
	virtual void				__stdcall	GXOEnableSchedule(GXOBJECT_HANDLE gxoh) = 0;
	
	virtual I3DModel*			__stdcall	GXOGetModel(GXOBJECT_HANDLE gxh) = 0;
	virtual void*				__stdcall	GXOGetData(GXOBJECT_HANDLE gxh) = 0;
	virtual void				__stdcall	GXOSetData(GXOBJECT_HANDLE gxh,void* pData) = 0;

	virtual GXOBJECT_HANDLE		__stdcall	GXOGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag) = 0;
	virtual	I3DModel*			__stdcall	PreLoadGXObject(char* szFileName,DWORD dwMaxRefNum) = 0;
	virtual	BOOL				__stdcall	PreCreateLight(char* szFileName,DWORD dwFlag) = 0;
	virtual GXOBJECT_HANDLE 	__stdcall	CreateGXObject(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag) = 0;
	virtual GXLIGHT_HANDLE		__stdcall	CreateGXLight(LIGHT_DESC* pDesc,GXLightSchedulePROC pProc,void* pData,DWORD dwProjTexIndex,DWORD dwFlag) = 0;
	virtual GXMAP_HANDLE		__stdcall	CreateGXMap(GXMapSchedulePROC pProc,void* pData,DWORD dwFlag) = 0;

	virtual BOOL				__stdcall	BeginBuildMap(DWORD dwFlag) = 0;
	virtual BOOL				__stdcall	InsertHFieldToGXMap(IHeightField* pHField) = 0;
	virtual BOOL				__stdcall	InsertStaticModelTOGXMap(I3DStaticModel* pModel) = 0;
	virtual BOOL				__stdcall	EndBuildMap() = 0;
	
	virtual BOOL				__stdcall	DeleteGXObject(GXOBJECT_HANDLE gxh) = 0;
	virtual BOOL				__stdcall	DeleteGXLight(GXLIGHT_HANDLE gxh) = 0;
	virtual BOOL				__stdcall	DeleteAllGXObjects() = 0;
	virtual BOOL				__stdcall	DeleteAllGXLights() = 0;
	virtual	BOOL				__stdcall	DeleteGXMap(GXMAP_HANDLE gxm) = 0;

	virtual DWORD				__stdcall	GetGXLightList(GXOBJECT_HANDLE gxh,LIGHT_DESC* pRelatedLight,DWORD dwMaxLightNum) = 0;
	virtual DWORD				__stdcall	Process() = 0;
	virtual DWORD				__stdcall	GetFramePerSec() = 0;
	virtual	void				__stdcall	SetFramePerSec(DWORD dwFrame) = 0;
	
	virtual	LIGHT_DESC*			__stdcall	GXLGetLightDesc(GXLIGHT_HANDLE gxh) = 0;
	virtual void				__stdcall	GXLSetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual	void				__stdcall	GXLMovePosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void				__stdcall	GXLGetPosition(GXLIGHT_HANDLE gxh,VECTOR3** ppv3Pos) = 0;
	virtual GXLIGHT_HANDLE		__stdcall	GXLGetLightWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag) = 0;
	
	virtual void*				__stdcall	GXMGetData(GXMAP_HANDLE gxh) = 0;
	virtual void				__stdcall	GXMSetData(GXMAP_HANDLE gxh,void* pData) = 0;
	virtual BOOL				__stdcall	GXMGetCollisionPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag) = 0;
	
	virtual I3DStaticModel*		__stdcall	GXMGetBaseModel(GXMAP_HANDLE gxh) = 0;
	virtual BOOL				__stdcall	GXMStaticShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag) = 0;
	
	virtual void				__stdcall	GXOAlignAllObjects() = 0;
	virtual	BOOL				__stdcall	SetSymbol(SYMBOL_TYPE type,char* szFileName,DWORD dwMaxRefNum) = 0;
	virtual	void				__stdcall	SetRenderMode(DWORD dwRenderMode) = 0;

};

#endif



