#pragma once
#include "../max_common/stdafx.h"
#include "../4DyuchiGRX_Common/motion_flag.h"

enum KEY_TYPE {KEY_TYPE_POS = 0x000000ff, KEY_TYPE_ROT = 0x0000ff00, KEY_TYPE_SCALE = 0x00ff0000};

struct ANIMATED_MESH_KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	DWORD			dwVertexNum;
	DWORD			dwTexVertexNum;
	VECTOR3*		pv3Pos;
	TVERTEX*		ptUV;

};

struct ROT_KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	QUARTERNION		Q;	
};

struct SCALE_KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	VECTOR3			v3Scale;
	VECTOR3			v3ScaleAxis;
	float			fScaleAxisAng;
};

struct POS_KEY 
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	VECTOR3			v3Pos;
};
struct FILE_MOTION_OBJECT_HEADER
{
	DWORD					dwIndex;
	
	DWORD					dwRotKeyNum;
	DWORD					dwPosKeyNum;
	DWORD					dwScaleKeyNum;
	DWORD					dwAnimatedMeshKeyNum;
	
	TCHAR					uszObjectName[MAX_NAME_LEN];
	CMotionFlag				motionFlag;

};

class CMotionObject
{	
	DWORD					m_dwIndex;
	
	DWORD					m_dwRotKeyNum;
	DWORD					m_dwPosKeyNum;
	DWORD					m_dwScaleKeyNum;
	DWORD					m_dwAnimatedMeshKeyNum;
	
	TCHAR					m_uszObjectName[MAX_NAME_LEN];
	CMotionFlag				m_motionFlag;

	ROT_KEY*				m_pRotKey;
	POS_KEY*				m_pPosKey;
	SCALE_KEY*				m_pScaleKey;
	ANIMATED_MESH_KEY*		m_pAnimatedMeshKey;
public:
	
	void					SetObjectName(TCHAR* uszObjName) {_tcscpy(m_uszObjectName,uszObjName);}
	TCHAR*					GetObjectName() {return m_uszObjectName;}

	BOOL					CreateRotKeyList(DWORD dwNum);
	BOOL					CreatePosKeyList(DWORD dwNum);
	BOOL					CreateScaleKeyList(DWORD dwNum);
	BOOL					CreateAnimatedMeshKeyList(DWORD dwNum);


	BOOL					InsertRotKey(ROT_KEY* pRotKey);
	BOOL					InsertPosKey(POS_KEY* pPosKey);
	BOOL					InsertScaleKey(SCALE_KEY* pScaleKey);
	BOOL					InsertAnimatedMeshKey(ANIMATED_MESH_KEY* pAnimMeshKey);
	
	void					ReleasePosKey();
	void					ReleaseRotKey();
	void					ReleaseScaleKey();

	DWORD					WriteFile(FILE* fp);
	DWORD					ReadFile(FILE* fp);
	CMotionObject();
	~CMotionObject();

	CMotionObject*			m_pNextMotionObject;
};
